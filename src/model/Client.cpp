#include <iostream>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include <string>
#include <exception>

#include "Client.h"
#include "../view/Nivel1Vista.h"
#include "../view/Nivel2Vista.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../TextRenderer.h"
#include "../StartPageView.h"
#include "../view/DesconexionVista.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "../controller/MarioController.h"
#include "../utils/dataTransfer.h"

#define SERVER_CONNECTION_SUCCESS 0
#define START_PAGE_SUCCESS 0

typedef struct handleLevelStateArgs
{
    int clientSocket;
    estadoNivel_t **estado;
} handleLevelStateArgs_t;

pthread_mutex_t mutex;
bool serverOpen = true;

void *sendDataThread(void *args);
void *receiveDataThread(void *args);

void getNextLevelView(NivelVista **vista, unsigned char currentLevel, SDL_Renderer *);

Client::Client(char *serverIp, char *port)
{
    std::cout << "Aplicación iniciada en modo cliente" << std::endl;

    this->serverIp = serverIp;
    this->port = port;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(NOMBRE_JUEGO.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

int Client::startClient()
{
    if (this->connectToServer() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (this->showStartPage() == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }

    if (serverOpen)
    {
    this->showConnectedPage();
    this->startGame();
    }

    if (!serverOpen)
    {
        DesconexionVista::show(renderer);
        while (!SDL_QuitRequested())
        {
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

int Client::connectToServer()
{
    std::cout << "Conectando al servidor: " << serverIp << " puerto: " << port << std::endl;

    //socket
    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        return EXIT_FAILURE;
    }

    serverAddress.sin_addr.s_addr = inet_addr(serverIp);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(port));

    //connect
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) != 0)
    {
        std::cout << "Error al conectarse con el servidor" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Client::startGame()
{
    logger::Logger::getInstance().logNewGame();

    auto configuration = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    auto log_level = configuration->getLogLevel();
    logger::Logger::getInstance().setLogLevel(log_level);

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
                getNextLevelView(&vista, ++currentLevel, renderer);
            SDL_RenderClear(renderer);
            vista->update(estadoNivel);
            estadoNivel = NULL;
            pthread_mutex_unlock(&mutex);
            SDL_RenderPresent(renderer);
        }
        quitRequested = SDL_QuitRequested();
    }

    logger::Logger::getInstance().logInformation("Game over");
}

void *sendDataThread(void *args)
{
    int clientSocket = *(int *)args;
    controls_t controls = getControls();

    bool quitRequested = false;
    while (!quitRequested && serverOpen)
    {

        if (*reinterpret_cast<char *>(&controls) != *reinterpret_cast<char *>(&(controls = getControls())))
        {
            if (sendData(clientSocket, &controls, sizeof(controls_t)) < sizeof(controls_t))
            {
                serverOpen = false;
            }
        }

        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return NULL;
}

void *receiveDataThread(void *args)
{
    int clientSocket = ((handleLevelStateArgs_t *)args)->clientSocket;
    estadoNivel_t **estado = ((handleLevelStateArgs_t *)args)->estado;
    estadoNivel_t view;

    bool quitRequested = false;
    while (!quitRequested && serverOpen)
    {
        if (receiveData(clientSocket, &view, sizeof(estadoNivel_t)) == sizeof(estadoNivel_t))
        {
            pthread_mutex_lock(&mutex);
            *estado = &view;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            serverOpen = false;
        }
        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return NULL;
}

void getNextLevelView(NivelVista **vista, unsigned char currentLevel, SDL_Renderer *renderer)
{
    auto config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
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
    StartPage startPage = StartPage(renderer);
    char response;
    try
    {
        do
        {
            user_t user = startPage.getLoginUser();
            response = login(user);

            if (response == LOGIN_OK)
            {
            }
            else
            {
                startPage.renderResponse(response);
            }
        } while (response != LOGIN_OK && serverOpen);
    }
    catch (std::exception &e)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

char Client::login(user_t user)
{
    char response;

    sendData(clientSocket, &user, sizeof(user_t));
    if (receiveData(clientSocket, &response, sizeof(char)) < sizeof(char))
    {
        serverOpen = false;
        return LOGIN_ABORTED;
    }

    return response;
}

void Client::showConnectedPage()
{
    SDL_RenderClear(renderer);

    punto_t pos;
    pos.x = (10 + 2) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = (110 + 2) * ALTO_PANTALLA / (float)ALTO_NIVEL;
    TextRenderer::getInstance(renderer)->renderText(pos, "Esperando a jugadores...", 1);

    SDL_RenderPresent(renderer);
}