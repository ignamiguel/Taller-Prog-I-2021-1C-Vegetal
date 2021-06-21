#include <iostream>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include <string>
#include "../view/Nivel1Vista.h"
#include "../view/Nivel2Vista.h"
#include "../controller/MarioController.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "../TextRenderer.h"
#include "Client.h"

typedef struct handleLevelStateArgs {
    int clientSocket;
    estadoNivel_t **estado;
} handleLevelStateArgs_t;

pthread_mutex_t mutex;

void getNextLevelView(NivelVista **vista, configuration::GameConfiguration *config, unsigned char currentLevel, SDL_Renderer *);

Client::Client() {
    std::cout << "Aplicación iniciada en modo cliente" << std::endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(NOMBRE_JUEGO.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

int Client::connectToServer(char* serverIp, char* port) {
    std::cout << "Intento conectarme al servidor en: " << serverIp << " en el puerto: " << port << std::endl;

    //socket
    clientSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (clientSocket == -1)
        return -1;

    serverAddress.sin_addr.s_addr = inet_addr(serverIp);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons( atoi(port));

    std::cout << "pre connect" << std::endl;
    //connect
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) < 0) {
        printf("error\n");
        return -1;
    }
    std::cout << "post connect" << std::endl;
    startGame();  
    
    return 1;
}

void Client::startGame() {
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

    pthread_t recieveThread;
    pthread_create(&recieveThread, NULL, recieveDataThread, &receiveArgs);

    bool quitRequested = false;
    while(!quitRequested) {
        if (estadoNivel != NULL) {
            pthread_mutex_lock(&mutex);
            if (currentLevel < estadoNivel->level) getNextLevelView(&vista, &configuration, ++currentLevel, renderer);
            SDL_RenderClear(renderer);
            vista->update(estadoNivel);
            estadoNivel = NULL;
            pthread_mutex_unlock(&mutex);
            SDL_RenderPresent(renderer);
        }
        quitRequested = SDL_QuitRequested();
    }

    logger::Logger::getInstance().logInformation("Game over");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void* Client::sendDataThread(void *args) {
    int clientSocket = *(int *)args;
    controls_t controls = getControls();

    bool quitRequested = false;
    while (!quitRequested) {
        if (*reinterpret_cast<char *>(&controls) != *reinterpret_cast<char *>(&(controls = getControls())))
            sendCommand(clientSocket, &controls);
        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return NULL;
}

int Client::sendCommand(int clientSocket, controls_t* controls) {
    size_t totalBytesSent = 0;
    int bytesSent = 0;
    size_t dataSize = sizeof(controls_t);
    bool clientSocketStillOpen = true;
    
    while((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = send(clientSocket, (controls + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
        if(bytesSent < 0) {
            return bytesSent;
        } 
        else if(bytesSent == 0) {
            clientSocketStillOpen = false;
        }
        else {
            totalBytesSent += bytesSent;
        }
    }

    return totalBytesSent;
}

void *Client::recieveDataThread(void *args) {
    int clientSocket = ((handleLevelStateArgs_t *)args)->clientSocket;
    estadoNivel_t **estado = ((handleLevelStateArgs_t *)args)->estado;
    estadoNivel_t view;
    int bytesReceived;

    bool quitRequested = false;
    while(!quitRequested) {
        bytesReceived = receiveView(clientSocket, &view);
        if (bytesReceived == sizeof(estadoNivel_t)) {
            pthread_mutex_lock(&mutex);
            *estado = &view;
            pthread_mutex_unlock(&mutex);
        }
        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return NULL;
}

int Client::receiveView(int clientSocket, estadoNivel_t* view) {
    size_t totalBytesReceived = 0;
    int bytesReceived = 0;
    size_t dataSize = sizeof(estadoNivel_t);
    bool clientSocketStillOpen = true;

    while((totalBytesReceived < dataSize) && clientSocketStillOpen) {
        bytesReceived = recv(clientSocket, (view + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
        if(bytesReceived < 0) {
            return bytesReceived;
        } 
        else if(bytesReceived == 0) {
            clientSocketStillOpen = false;
        }
        else {
            totalBytesReceived += bytesReceived;
        }
    }

    return totalBytesReceived;
}

void getNextLevelView(NivelVista **vista, configuration::GameConfiguration *config, unsigned char currentLevel, SDL_Renderer *renderer) {
    int maxPlayers = config->getMaxPlayers();
    if(maxPlayers < 0)
        maxPlayers = DEFAULT_MAX_PLAYERS;
    
    delete *vista;
    if (currentLevel == 1) {
        *vista = new Nivel1Vista(renderer, config->getDefaultConfigFlag());
        (*vista)->addPlayers(maxPlayers);
        auto stages = config->getStages();
        if (stages.size() > 0) {
            std::string rutaImagen = stages[0].getBackgrounds()[0];
            logger::Logger::getInstance().logDebug("Stage 1 background img: " + rutaImagen);
            (*vista)->setBackground(rutaImagen);
        }
    }
    if (currentLevel == 2) {
        *vista = new Nivel2Vista(renderer, config->getDefaultConfigFlag());
        (*vista)->addPlayers(maxPlayers);
        auto stages = config->getStages();
        if (stages.size() > 1) {
            std::string rutaImagen = stages[1].getBackgrounds()[0];
            logger::Logger::getInstance().logDebug("Stage 2 background img: " + rutaImagen);
            (*vista)->setBackground(rutaImagen);
        }
    }
}

const float RESIZE = 1.2;
const char* IMG_FONT = "res/font.png";
const float TEXT_X = 10;
const float TEXT_Y = 110;

void Client::showWaitingView() {
    SDL_RenderClear(renderer);
    TextRenderer* textRenderer = new TextRenderer(renderer, IMG_FONT);
    
    punto_t pos;
    pos.x = (10 + 2 * RESIZE) * ANCHO_PANTALLA / (float)ANCHO_NIVEL;
    pos.y = (110 + 2 * RESIZE)* ALTO_PANTALLA / (float)ALTO_NIVEL;
    textRenderer->renderText(pos, "Esperando a jugadores...", RESIZE);

    SDL_RenderPresent(renderer);
}