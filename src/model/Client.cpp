#include <iostream>
#include <pthread.h>
#include <string>

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
#include "../utils/estadoJuego.h"
#include "../controller/MarioController.h"
#include "../utils/dataTransfer.h"
#include "../controller/AudioController.h"
#include "../utils/Messages.h"

#define SERVER_CONNECTION_SUCCESS 0
#define START_PAGE_SUCCESS 0

typedef struct handleLevelStateArgs
{
    int clientSocket;
    const estadoJuego_t *&estado;
} handleLevelStateArgs_t;

pthread_mutex_t mutex;
bool serverOpen = true;

void *sendDataThread(void *args);
void *receiveDataThread(void *args);

Client::Client(char *serverIp, char *port)
{
    std::cout << "Aplicación iniciada en modo cliente\n";

    this->serverIp = serverIp;
    this->port = port;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(NOMBRE_JUEGO.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    AudioController::loadAudioFiles();
}

int Client::startClient()
{
    if (this->connectToServer() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if (this->showStartPage() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    if (serverOpen) {
        this->showConnectedPage();
        ClientExitStatus exitStatus = this->startGame();
        processExit(exitStatus);
    } else {
        processExit(CLIENT_CONNECTION_CLOSED);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}

void Client::processExit(ClientExitStatus clientExitStatus) {
    switch (clientExitStatus) {
        case CLIENT_GAME_OVER:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "GAME_OVER");
            this->showGameOverPage();
            break;
        case CLIENT_CONNECTION_CLOSED:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "CONNECTION_CLOSED");
            DesconexionVista::show(renderer);
            break;
        case CLIENT_QUIT_REQUESTED:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "QUIT_REQUESTED");
            break;
        case CLIENT_GAME_COMPLETED:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "CLIENT_GAME_COMPLETED");
            this->showGameCompletedPage();
            break;
        default:
            break;
    }
    while (!SDL_QuitRequested()) { };
}

int Client::connectToServer()
{
    std::cout << "Conectando al servidor: " << serverIp << " puerto: " << port << '\n';

    //socket
    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        return EXIT_FAILURE;
    }

    serverAddress.sin_addr.s_addr = inet_addr(serverIp);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(port));

    //connect
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) != 0) {
        std::cout << "Error al conectarse con el servidor\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

ClientExitStatus Client::startGame()
{
    AudioController::toggleMusic();

    logger::Logger::getInstance().logNewGame();

    auto configuration = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    auto log_level = configuration->getLogLevel();
    logger::Logger::getInstance().setLogLevel(log_level);

    unsigned char currentLevel = 0;
    NivelVista *vista{nullptr};

    pthread_t sendThread;
    pthread_create(&sendThread, NULL, sendDataThread, &clientSocket);

    const estadoJuego_t *estadoJuego{nullptr};

    handleLevelStateArgs_t receiveArgs{clientSocket, estadoJuego};

    pthread_t receiveThread;
    pthread_create(&receiveThread, NULL, receiveDataThread, &receiveArgs);

    bool quitRequested = false;
    bool isGameOver = false;
    bool isGameCompleted = false;

    while (!quitRequested && serverOpen && !isGameOver && !isGameCompleted) {
        if (estadoJuego != nullptr) {
            pthread_mutex_lock(&mutex);
            if (currentLevel < estadoJuego->estadoNivel.level)
                getNextLevelView(vista, ++currentLevel);
            SDL_RenderClear(renderer);
            vista->update(*estadoJuego);

            isGameOver = estadoJuego->estadoNivel.isGameOver;
            isGameCompleted = estadoJuego->estadoNivel.isGameCompleted 
                && estadoJuego->estadoNivel.level == NIVEL_2;

            estadoJuego = nullptr;
            pthread_mutex_unlock(&mutex);
            SDL_RenderPresent(renderer);
        }

        AudioController::checkToggleMusicEvent();
        quitRequested = SDL_QuitRequested();
    }

    if (isGameOver) return CLIENT_GAME_OVER;
    if (isGameCompleted) return CLIENT_GAME_COMPLETED;
    if (!serverOpen) return CLIENT_CONNECTION_CLOSED;
    return CLIENT_QUIT_REQUESTED;
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
            if (sendData(clientSocket, &controls) < sizeof(controls_t))
                serverOpen = false;
        }

        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return nullptr;
}

void *receiveDataThread(void *args)
{
    int clientSocket = ((handleLevelStateArgs_t *)args)->clientSocket;
    const estadoJuego_t *&estado = ((handleLevelStateArgs_t *)args)->estado;
    estadoJuego_t game;

    bool quitRequested = false;
    while (!quitRequested && serverOpen) {
        if (receiveData(clientSocket, &game) == sizeof(estadoJuego_t)) {
            pthread_mutex_lock(&mutex);
            estado = &game;
            pthread_mutex_unlock(&mutex);
        } else {
            serverOpen = false;
        }
        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return nullptr;
}

void Client::getNextLevelView(NivelVista *&vista, unsigned char currentLevel)
{
    delete vista;
    if (currentLevel == 1)
    {
        vista = new Nivel1Vista(renderer, name);
    }
    if (currentLevel == 2)
    {
        vista = new Nivel2Vista(renderer, name);
    }
}

int Client::showStartPage()
{
    StartPage startPage{renderer};
    int response;
    bool quitRequested = false;
    do
    {
        user_t user = startPage.getLoginUser(quitRequested);
        if (quitRequested) {
            return EXIT_FAILURE;
        }

        response = login(user);
        if (response == LOGIN_OK) {
            strcpy(this->name, user.username);
        } else {
            startPage.setResponse(response);
        }
    } while (response != LOGIN_OK && serverOpen);

    return EXIT_SUCCESS;
}

int Client::login(user_t user)
{
    char response;

    sendData(clientSocket, &user);
    if (receiveData(clientSocket, &response) < sizeof(char))
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
    pos.x = (10 + 2) * (ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    pos.y = (110 + 2) * (ALTO_PANTALLA / (float)ALTO_NIVEL);
    TextRenderer::getInstance(renderer)->renderText(pos, MSG_ESPERANDO_JUGADORES, 1);

    SDL_RenderPresent(renderer);
}

void Client::showGameOverPage()
{
    SDL_RenderClear(renderer);

    punto_t pos;
    pos.x = (30 + 2) * (ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    pos.y = (110 + 2) * (ALTO_PANTALLA / (float)ALTO_NIVEL);
    TextRenderer::getInstance(renderer)->renderText(pos, MSG_GAME_OVER, 2);

    SDL_RenderPresent(renderer);
}

void Client::showGameCompletedPage()
{
    SDL_RenderClear(renderer);

    punto_t pos;
    pos.x = (42) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = 112 * ALTO_PANTALLA / (float)ALTO_NIVEL;
    TextRenderer::getInstance(renderer)->renderText(pos, MSG_FELICITACIONES, 1);
    pos.x = (42) * (ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    pos.y = (122) * (ALTO_PANTALLA / (float)ALTO_NIVEL);
    TextRenderer::getInstance(renderer)->renderText(pos, MSG_JUEGO_COMPLETADO, 1);

    SDL_RenderPresent(renderer);
}