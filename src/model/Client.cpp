#include <iostream>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include <string>
#include <exception>
#include "../view/Nivel1Vista.h"
#include "../view/Nivel2Vista.h"
#include "../controller/MarioController.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "../TextRenderer.h"
#include "Client.h"
#include "../StartPageView.h"
#include "../view/DesconexionVista.h"

#define SERVER_CONNECTION_SUCCESS 0
#define START_PAGE_SUCCESS 0

const char *IMG_FONT = "res/font.png";

typedef struct handleLevelStateArgs
{
    int clientSocket;
    estadoNivel_t **estado;
} handleLevelStateArgs_t;

pthread_mutex_t mutex;
bool serverOpen = true;
void getNextLevelView(NivelVista **vista, configuration::GameConfiguration *config, unsigned char currentLevel, SDL_Renderer *);

Client::Client(char *serverIp, char *port)
{
    this->serverIp = serverIp;
    this->port = port;
    std::cout << "Aplicación iniciada en modo cliente" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(NOMBRE_JUEGO.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

int Client::startClient()
{
    if (this->connectToServer() != SERVER_CONNECTION_SUCCESS)
    {
        std::cout << "Hubo un error al connectarse al servidor" << std::endl;
        return -1;
    }

    if (this->showStartPage() != START_PAGE_SUCCESS)
    {
        std::cout << "Hubo un error con StartPage" << std::endl;
        return -1;
    }

    this->showConnectedPage();

    this->startGame();
    return 0;
}

int Client::connectToServer()
{
    std::cout << "Conectando al servidor: " << serverIp << " puerto: " << port << std::endl;

    //socket
    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        return -1;
    }

    serverAddress.sin_addr.s_addr = inet_addr(serverIp);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(port));

    std::cout << "Conectado" << std::endl;
    //connect
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) == 0)
    {
        std::cout << "Conectado al servidor" << std::endl;
    }
    else
    {
        std::cout << "Error al conectarse con el servidor" << std::endl;
        return -1;
    }

    if (!serverOpen)
    {
        std::cout << "Hubo un error en el servidor" << std::endl;
        return -1;
    }

    return 0;
}

void Client::startGame()
{
    logger::Logger::getInstance().logNewGame();

    auto configuration = configuration::GameConfiguration(CONFIG_FILE);
    auto log_level = configuration.getLogLevel();
    logger::Logger::getInstance().setLogLevel(log_level);

    srand(time(NULL));

    unsigned char currentLevel = 0;
    NivelVista *vista = NULL;

    pthread_t sendThread;
    pthread_create(&sendThread, NULL, sendDataThread, &clientSocket);

    estadoNivel_t *estadoNivel = NULL;

    handleLevelStateArgs_t receiveArgs;
    receiveArgs.clientSocket = clientSocket;
    receiveArgs.estado = &estadoNivel;

    pthread_t receiveThread;
    pthread_create(&receiveThread, NULL, receiveDataThread, &receiveArgs);

    bool quitRequested = false;
    while (!quitRequested && serverOpen)
    {
        if (estadoNivel != NULL)
        {
            pthread_mutex_lock(&mutex);
            if (currentLevel < estadoNivel->level)
                getNextLevelView(&vista, &configuration, ++currentLevel, renderer);
            SDL_RenderClear(renderer);
            vista->update(estadoNivel);
            estadoNivel = NULL;
            pthread_mutex_unlock(&mutex);
            SDL_RenderPresent(renderer);
        }
        quitRequested = SDL_QuitRequested();
    }

    logger::Logger::getInstance().logInformation("Game over");
    if(!serverOpen) {
        quitRequested = false;
        DesconexionVista::show(renderer);
        while(!quitRequested) {
            quitRequested = SDL_QuitRequested();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void *Client::sendDataThread(void *args)
{
    int clientSocket = *(int *)args;
    controls_t controls = getControls();

    bool quitRequested = false;
    while (!quitRequested && serverOpen)
    {

        if (*reinterpret_cast<char *>(&controls) != *reinterpret_cast<char *>(&(controls = getControls())))
        {
            int bytesSent = sendCommand(clientSocket, &controls);
            if (bytesSent <= 0)
                serverOpen = false;
        }

        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return NULL;
}

int Client::sendCommand(int clientSocket, controls_t *controls)
{
    size_t totalBytesSent = 0;
    int bytesSent = 0;
    size_t dataSize = sizeof(controls_t);
    bool clientSocketStillOpen = true;

    while ((totalBytesSent < dataSize) && clientSocketStillOpen)
    {
        bytesSent = send(clientSocket, (controls + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
        if (bytesSent < 0)
        {
            return bytesSent;
        }
        else if (bytesSent == 0)
        {
            clientSocketStillOpen = false;
        }
        else
        {
            totalBytesSent += bytesSent;
        }
    }

    return totalBytesSent;
}

void *Client::receiveDataThread(void *args)
{
    int clientSocket = ((handleLevelStateArgs_t *)args)->clientSocket;
    estadoNivel_t **estado = ((handleLevelStateArgs_t *)args)->estado;
    estadoNivel_t view;
    int bytesReceived;

    bool quitRequested = false;
    while (!quitRequested && serverOpen)
    {
        bytesReceived = receiveView(clientSocket, &view);
        if (bytesReceived == 0)
            serverOpen = false;
        if (bytesReceived == sizeof(estadoNivel_t))
        {
            pthread_mutex_lock(&mutex);
            *estado = &view;
            pthread_mutex_unlock(&mutex);
        }
        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return NULL;
}

int Client::receiveView(int clientSocket, estadoNivel_t *view)
{
    size_t totalBytesReceived = 0;
    int bytesReceived = 0;
    size_t dataSize = sizeof(estadoNivel_t);
    bool clientSocketStillOpen = true;

    while ((totalBytesReceived < dataSize) && clientSocketStillOpen)
    {
        bytesReceived = recv(clientSocket, (view + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
        if (bytesReceived < 0)
        {
            return bytesReceived;
        }
        else if (bytesReceived == 0)
        {
            clientSocketStillOpen = false;
        }
        else
        {
            totalBytesReceived += bytesReceived;
        }
    }

    return totalBytesReceived;
}

void getNextLevelView(NivelVista **vista, configuration::GameConfiguration *config, unsigned char currentLevel, SDL_Renderer *renderer)
{
    int maxPlayers = config->getMaxPlayers();
    if (maxPlayers < 0)
        maxPlayers = DEFAULT_MAX_PLAYERS;

    delete *vista;
    if (currentLevel == 1)
    {
        *vista = new Nivel1Vista(renderer, config->getDefaultConfigFlag());
        (*vista)->addPlayers(maxPlayers);
        auto stages = config->getStages();
        if (stages.size() > 0)
        {
            std::string rutaImagen = stages[0].getBackgrounds()[0];
            logger::Logger::getInstance().logDebug("Stage 1 background img: " + rutaImagen);
            (*vista)->setBackground(rutaImagen);
        }
    }
    if (currentLevel == 2)
    {
        *vista = new Nivel2Vista(renderer, config->getDefaultConfigFlag());
        (*vista)->addPlayers(maxPlayers);
        auto stages = config->getStages();
        if (stages.size() > 1)
        {
            std::string rutaImagen = stages[1].getBackgrounds()[0];
            logger::Logger::getInstance().logDebug("Stage 2 background img: " + rutaImagen);
            (*vista)->setBackground(rutaImagen);
        }
    }
}

int Client::showStartPage()
{
    StartPage *startPage = new StartPage(renderer);
    int response;
    try
    {
        do
        {
            user_t user = startPage->getLoginUser();
            response = login(user);

            if (response == LOGIN_OK)
            {
                this->user = user;
            }

            startPage->renderResponse(response);

        } while (response != LOGIN_OK);
    }
    catch (std::exception &e)
    {
        delete startPage;
        return -1;
    }
    delete startPage;
    return 0;
}

void Client::showConnectedPage()
{

    SDL_RenderClear(renderer);

    TextRenderer *textRenderer = new TextRenderer(renderer, IMG_FONT);

    punto_t pos;
    pos.x = (10 + 2) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = (110 + 2) * ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, "Esperando a jugadores...", 1);

    SDL_RenderPresent(renderer);
    delete textRenderer;
}

int Client::login(user_t user)
{
    int response;

    int bytesSent = sendLoginRequest(&user);
    std::cout << "Sent " << bytesSent << std::endl;

    int bytesReceived = receiveLoginResponse(&response);
    std::cout << "Received " << bytesReceived << std::endl;
    std::cout << "Response " << response << std::endl;

    if (bytesReceived != sizeof(int))
    {
        // TODO: bytesReceived is not int
        return 0;
    }

    return response;
}

int Client::sendLoginRequest(user_t *user)
{
    int totalBytesSent = 0;
    int bytesSent = 0;
    int dataSize = sizeof(user_t);
    bool clientSocketStillOpen = true;

    while ((totalBytesSent < dataSize) && clientSocketStillOpen)
    {
        bytesSent = send(this->clientSocket, (user + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
        if (bytesSent < 0)
        {
            return bytesSent;
        }
        else if (bytesSent == 0)
        {
            clientSocketStillOpen = false;
        }
        else
        {
            totalBytesSent += bytesSent;
        }
    }
    return totalBytesSent;
}

int Client::receiveLoginResponse(int *response)
{
    int totalBytesReceived = 0;
    int bytesReceived = 0;
    int dataSize = sizeof(int);
    bool clientSocketStillOpen = true;

    while ((totalBytesReceived < dataSize) && clientSocketStillOpen)
    {
        bytesReceived = recv(clientSocket, (response + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
        if (bytesReceived < 0)
        {
            return bytesReceived;
        }
        else if (bytesReceived == 0)
        {
            clientSocketStillOpen = false;
        }
        else
        {
            totalBytesReceived += bytesReceived;
        }
    }

    return totalBytesReceived;
}
