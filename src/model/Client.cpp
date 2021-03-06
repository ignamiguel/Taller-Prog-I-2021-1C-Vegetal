#include <iostream>
#include <pthread.h>
#include <string>
#include <SDL2/SDL_image.h>

#include "Client.h"
#include "../view/Nivel1Vista.h"
#include "../view/Nivel2Vista.h"
#include "../view/InterludeVista.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../TextRenderer.h"
#include "../StartPageView.h"
#include "../view/showMessage.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "../utils/estadoJuego.h"
#include "../controller/MarioController.h"
#include "../utils/dataTransfer.h"
#include "../controller/AudioController.h"

#define SERVER_CONNECTION_SUCCESS 0
#define START_PAGE_SUCCESS 0

typedef struct handleLevelStateArgs
{
    int clientSocket;
    const estadoJuego_t *&estado;
} handleLevelStateArgs_t;

pthread_mutex_t mutex;
bool serverOpen = true;
bool quitRequested = false;

void *sendDataThread(void *args);
void *receiveDataThread(void *args);

Client::Client(char *serverIp, char *port)
{
    std::cout << "Aplicación iniciada en modo cliente\n";

    this->serverIp = serverIp;
    this->port = port;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(NOMBRE_JUEGO, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    AudioController::loadAudioFiles();
}

Client::~Client() {
    AudioController::closeAudioFiles();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
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
        showMessage::waitingLobby(renderer);
        ExitStatus exitStatus = this->startGame();
        processExit(exitStatus);
    } else {
        processExit(CLIENT_CONNECTION_CLOSED);
    }

    return EXIT_SUCCESS;
}

void Client::processExit(ExitStatus exitStatus) {
    switch (exitStatus) {
        case CLIENT_CONNECTION_CLOSED:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "CONNECTION CLOSED");
            showMessage::disconnection(renderer);
            break;
        case CLIENT_GAME_OVER:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "GAME OVER");
            showMessage::gameOver(renderer);
            break;
        case CLIENT_GAME_COMPLETE:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "GAME COMPLETE");
            showMessage::gameComplete(renderer);
            break;
        case CLIENT_QUIT_REQUESTED:
            logger::Logger::getInstance().logInformation(std::string("[") + this->name + "] " + "QUIT REQUESTED");
            return;
        default:
            break;
    }
    while (!SDL_QuitRequested()) {
        AudioController::checkToggleMusicEvent();
    };
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

ExitStatus Client::startGame()
{
    AudioController::toggleMusic();

    logger::Logger::getInstance().logNewGame();

    auto configuration = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    auto log_level = configuration->getLogLevel();
    logger::Logger::getInstance().setLogLevel(log_level);

    unsigned char currentScene{};
    SceneVista *vista{nullptr};

    pthread_t sendThread;
    pthread_create(&sendThread, NULL, sendDataThread, &clientSocket);

    const estadoJuego_t *estadoJuego{nullptr};

    handleLevelStateArgs_t receiveArgs{clientSocket, estadoJuego};

    pthread_t receiveThread;
    pthread_create(&receiveThread, NULL, receiveDataThread, &receiveArgs);

    ExitStatus exitStatus = CLIENT_CONNECTION_CLOSED;

    while (!quitRequested && serverOpen) {
        if (estadoJuego != nullptr) {
            if (currentScene == estadoJuego->estadoNivel.sceneNumber) {
                SDL_RenderClear(renderer);

                pthread_mutex_lock(&mutex);
                vista->update(*estadoJuego);
                estadoJuego = nullptr;
                pthread_mutex_unlock(&mutex);

                SDL_RenderPresent(renderer);
            } else {
                currentScene = estadoJuego->estadoNivel.sceneNumber;
                getSceneView(vista, estadoJuego->estadoNivel.sceneNumber);
                exitStatus = static_cast<ExitStatus>(estadoJuego->estadoNivel.exitStatus);
            }
        }

        AudioController::checkToggleMusicEvent();
        quitRequested = SDL_QuitRequested();
    }
    shutdown(clientSocket, SHUT_RDWR);
    pthread_join(sendThread, NULL);
    pthread_join(receiveThread, NULL);
    delete vista;
    return quitRequested ? CLIENT_QUIT_REQUESTED : exitStatus;
}

void *sendDataThread(void *args)
{
    int clientSocket = *(int *)args;
    controls_t controls = MarioController::getControls();

    while (!quitRequested && serverOpen)
    {

        if (*reinterpret_cast<char *>(&controls) != *reinterpret_cast<char *>(&(controls = MarioController::getControls())))
        {
            if (sendData(clientSocket, &controls) < sizeof(controls_t))
                serverOpen = false;
        }
    }
    return nullptr;
}

void *receiveDataThread(void *args)
{
    int clientSocket = ((handleLevelStateArgs_t *)args)->clientSocket;
    const estadoJuego_t *&estado = ((handleLevelStateArgs_t *)args)->estado;
    estadoJuego_t game;

    while (!quitRequested && serverOpen) {
        if (receiveData(clientSocket, &game) == sizeof(estadoJuego_t)) {
            pthread_mutex_lock(&mutex);
            estado = &game;
            pthread_mutex_unlock(&mutex);
        } else {
            serverOpen = false;
        }
    }
    return nullptr;
}

void Client::getSceneView(SceneVista *&vista, unsigned char serverScene)
{
    delete vista;
    switch (serverScene) {
        case 1:
            vista = new Nivel1Vista(renderer, name);
            break;
        case 2:
            vista = new InterludeVista(renderer);
            break;
        case 3:
            vista = new Nivel2Vista(renderer, name);
            break;
        case 4:
            vista = new InterludeVista(renderer);
            break;
        default:
            vista = nullptr;
    }
}

int Client::showStartPage()
{
    StartPage startPage{renderer};
    int response;
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