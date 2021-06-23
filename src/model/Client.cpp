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
#include "Client.h"
#include "../view/StartPageView.h"

typedef struct handleLevelStateArgs {
    int clientSocket;
    estadoNivel_t **estado;
} handleLevelStateArgs_t;

pthread_mutex_t mutex;
bool serverOpen = true;
void getNextLevelView(NivelVista **vista, configuration::GameConfiguration *config, unsigned char currentLevel, SDL_Renderer *);

Client::Client() {
    std::cout << "Aplicación iniciada en modo cliente" << std::endl;
}

void Client::startClient(char* serverIp, char* port) {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(NOMBRE_JUEGO.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    
    int connected = connectToServer(serverIp, port);
    if ( connected == 0 && (login() == LOGIN_OK) ) startGame();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int Client::connectToServer(char* serverIp, char* port) {
    std::cout << "Intento conectarme al servidor en: " << serverIp << " en el puerto: " << port << std::endl;

    //socket
    clientSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (clientSocket == -1) {
        return -1;
    }

    serverAddress.sin_addr.s_addr = inet_addr(serverIp);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons( atoi(port));

    std::cout << "Conectado" << std::endl;
    //connect
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)) < 0) {
        std::cout << "Error al conectarse con el servidor" << std::endl;
        return -1;
    }
    return 0;
}

// LOGIN

int Client::login() {
    std::cout << "start page" << std::endl;
    StartPage* startPage = new StartPage(renderer);
    int response;
    try {
        do {
            std::cout << "start login" << std::endl;
            user_t player = startPage->startLogin();
            std::cout << "player" << player.username << "-" << player.password << std::endl;
            requireLogin (&player);
            
            int bytesReceived = receiveLoginResponse(&response);
            std::cout << "received " << bytesReceived << std::endl;
            if(bytesReceived == sizeof(int)) {
                std::cout << "login response" << response << std::endl;
                if (response == LOGIN_OK) startPage->connectedPage();
                else startPage->connectionErrorMessage(response);
            }
        } while (response != LOGIN_OK);
    }
    catch (exception& e) {
        return LOGIN_ABORTED;
    }
    return response;
}

int Client::requireLogin (user_t* player) {
    int totalBytesSent = 0;
    int bytesSent = 0;
    int dataSize = sizeof(user_t);
    bool clientSocketStillOpen = true;
    
    while((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = send(clientSocket, (player + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
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

int Client::receiveLoginResponse (int* response) {
    int totalBytesReceived = 0;
    int bytesReceived = 0;
    int dataSize = sizeof(int);
    bool clientSocketStillOpen = true;

    while((totalBytesReceived < dataSize) && clientSocketStillOpen) {
        bytesReceived = recv(clientSocket, (response + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
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

// GAME

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

    pthread_t receiveThread;
    pthread_create(&receiveThread, NULL, receiveDataThread, &receiveArgs);

    bool quitRequested = false;
    while(!quitRequested && serverOpen) {
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
}

void* Client::sendDataThread(void *args) {
    int clientSocket = *(int *)args;
    controls_t controls = getControls();

    bool quitRequested = false;
    while(!quitRequested && serverOpen) {
        if (*reinterpret_cast<char *>(&controls) != *reinterpret_cast<char *>(&(controls = getControls()))) {
            int bytesSent = sendCommand(clientSocket, &controls);
            if(bytesSent <= 0) 
                serverOpen = false;
        }
            
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

void *Client::receiveDataThread(void *args) {
    int clientSocket = ((handleLevelStateArgs_t *)args)->clientSocket;
    estadoNivel_t **estado = ((handleLevelStateArgs_t *)args)->estado;
    estadoNivel_t view;
    int bytesReceived;

    bool quitRequested = false;
    while(!quitRequested && serverOpen) {
        bytesReceived = receiveView(clientSocket, &view);
        if(bytesReceived == 0)
            serverOpen = false;
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
