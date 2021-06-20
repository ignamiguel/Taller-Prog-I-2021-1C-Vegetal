#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include "../configuration.hpp"
#include "../logger.h"
#include "Nivel1.h"
#include "Nivel2.h"
#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"
#include "../utils/estadoNivel.h"
#include "Server.h"

typedef struct handleCommandArgs {
    int clientSocket;
    Mario* mario;
} handleCommandArgs_t;

void getNextLevel(Nivel **nivel, configuration::GameConfiguration *config, Uint8 currentLevel);

const int MAX_QUEUED_CONNECTIONS = 3;

Server::Server(char* port) {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(atoi(port));

    std::cout << "Aplicación iniciada en modo servidor en el puerto: " << port << std::endl;
}
int Server::startServer() {
    //socket
    int serverSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (serverSocket == -1) {
        return -1;
        // TODO: No es posible inicializar el socket
    }

    //bind
    int serverBind = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if(serverBind < 0) {
        return -1;
        // TODO: No es posible hacer el bind
    }

    //listen
    if (listen(serverSocket , MAX_QUEUED_CONNECTIONS) < 0){
        return -1;
        // TODO: listen devolvió menor a cero
    }

    printf("listen...\n");

    //Accept
    while(clientSockets.size() < MAX_PLAYERS) {
        int client = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t*) &clientAddrLen);
        clientSockets.push_back(client);
        printf("Players: %d/%d\n", (int)clientSockets.size(), MAX_PLAYERS);
    }

    if (clientSockets[0] < 0 || clientSockets[1] < 0){
         return -1;
         // TODO: agregar mensaje de error
    }

    printf("accept\n");

    startGame();

    for (int clientSocket : clientSockets) close(clientSocket);
    close(serverSocket);
    return 0;
}

void Server::startGame() {
    
    logger::Logger::getInstance().logNewGame();
    
    auto configuration = configuration::GameConfiguration(CONFIG_FILE);
    auto log_level = configuration.getLogLevel();
    logger::Logger::getInstance().setLogLevel(log_level);

    srand(time(NULL));
    SDL_Init(SDL_INIT_TIMER);

    std::vector<Mario*> players;
    for(unsigned int i = 0; i < MAX_PLAYERS; ++i) {
        players.push_back(new Mario());
    }

    Uint8 currentLevel = 0;
    Nivel *nivel = NULL;

    getNextLevel(&nivel, &configuration, ++currentLevel);
    nivel->addPlayers(&players);

    {
        handleCommandArgs_t handleCommandArgs[MAX_PLAYERS];
        for(unsigned int i = 0; i < clientSockets.size(); ++i) {
            handleCommandArgs[i].clientSocket = clientSockets[i];
            handleCommandArgs[i].mario = players[i];

            pthread_t recvCommandThread;
            pthread_create(&recvCommandThread, NULL, handleCommand, &handleCommandArgs[i]);
        }
    }

    Uint32 previous, current, elapsed, lag;
    bool updated, quitRequested = false;
    previous = SDL_GetTicks();
    lag = 0;
    while(!quitRequested) {
        current = SDL_GetTicks();
        elapsed = current - previous;
        previous = current;
        lag += elapsed;

        // Update Model
        updated = false;
        while (lag >= MS_PER_UPDATE) {
            nivel->update();
            lag -= MS_PER_UPDATE;
            updated = true;
        }

        if (updated) {
            estadoNivel_t* view = nivel->getEstado();
            for(unsigned int i = 0; i < clientSockets.size(); i++) sendView(clientSockets[i], view);
            if (nivel->isComplete()) {
                getNextLevel(&nivel, &configuration, ++currentLevel);
                if (nivel == NULL) {
                    SDL_Event e;
                    e.type = SDL_QUIT;
                    e.quit.timestamp = SDL_GetTicks();
                    SDL_PushEvent(&e);
                    break;
                }
                nivel->addPlayers(&players);
            }
        }
        quitRequested = SDL_QuitRequested();
    }
}

void *Server::handleCommand(void *handleCommandArgs) {
    Mario *player = ((handleCommandArgs_t *)handleCommandArgs)->mario;
    int clientSocket = ((handleCommandArgs_t *)handleCommandArgs)->clientSocket;

    controls_t controls;
    int bytesReceived;

    bool quitRequested = false;

    while(!quitRequested) {
        bytesReceived = receiveCommand(clientSocket, &controls);
        if (bytesReceived == sizeof(controls_t)) {
            player->setControls(controls);
        }

        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    return NULL;
}

int Server::sendView(int clientSocket, estadoNivel_t* view) {
    size_t totalBytesSent = 0;
    int bytesSent = 0;
    size_t dataSize = sizeof(estadoNivel_t);
    bool clientSocketStillOpen = true;
    
    while((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = send(clientSocket, (view + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
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

int Server::receiveCommand(int clientSocket, controls_t* controls) {
    size_t totalBytesSent = 0;
    int bytesSent = 0;
    size_t dataSize = sizeof(controls_t);
    bool clientSocketStillOpen = true;
    
    while((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = recv(clientSocket, (controls + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
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

void getNextLevel(Nivel **nivel, configuration::GameConfiguration *config, Uint8 currentLevel) {
    delete *nivel;
    if (currentLevel == 1) {
        logger::Logger::getInstance().logInformation("Level 1 starts");

        Nivel1 *nivel1 = new Nivel1();

        auto enemies = config->getEnemies();
        for (auto enemy: enemies) {
            if (enemy.getType().compare("Fuego") == 0) nivel1->addEnemies(enemy.getQuantity());
            logger::Logger::getInstance().logDebug("Enemy type: " + enemy.getType());
            logger::Logger::getInstance().logDebug("Enemy quantity: " + std::to_string(enemy.getQuantity()));
        }
        *nivel = nivel1;
    }
    else if (currentLevel == 2) {
        logger::Logger::getInstance().logInformation("End of Level 1");
        *nivel = new Nivel2();
        logger::Logger::getInstance().logInformation("Level 2 starts");
    }
    else {
        logger::Logger::getInstance().logInformation("End of Level 2");
        delete *nivel;
        *nivel = NULL;
    }
}