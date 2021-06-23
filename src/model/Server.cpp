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
#include "../utils/Messages.h"
#include "../utils/window.hpp"
#include "../utils/estadoNivel.h"
#include "Server.h"

typedef struct connectToClientArgs {
    std::vector<int>* clientSockets;
    int serverSocket;
    std::vector<user_t> users; 
    struct sockaddr_in* clientAddress;
    int* clientAddrLen;
    Server* server;
} connectToClientArgs_t;

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
    auto config = configuration::GameConfiguration(CONFIG_FILE);
    auto logLevel = config.getLogLevel();
    logger::Logger::getInstance().setLogLevel(logLevel);

    this->maxPlayers = config.getMaxPlayers();
    if(this->maxPlayers < 1 || this->maxPlayers > DEFAULT_MAX_PLAYERS) {
        logger::Logger::getInstance().logDebug(CANTIDAD_DE_JUGADORES_INVALIDA);
        this->maxPlayers = DEFAULT_MAX_PLAYERS;
    }

    //socket
    int serverSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (serverSocket == -1) {
        return -1;
    }

    //bind
    int serverBind = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if(serverBind < 0) {
        return -1;
    }

    //listen
    if (listen(serverSocket , MAX_QUEUED_CONNECTIONS) < 0) {
        return -1;
    }

    printf("listen...\n");

    //Accept
    
    connectToClientArgs_t arguments;
    arguments.clientSockets = &clientSockets;
    arguments.serverSocket = serverSocket;
    arguments.users = config.getUsers();
    arguments.clientAddress = &clientAddress;
    arguments.clientAddrLen = &clientAddrLen;
    arguments.server = this;

    std::cout << "players" << maxPlayers << " " << (int)maxPlayers << std::endl;
    
    for (int i =0; i < (int)maxPlayers; i++) {
        pthread_t clientConnection;
        std::cout << "thread creado" << std::endl;
        pthread_create(&clientConnection, NULL, connectToClient, &arguments);
    }

    std::cout << " waiting for players to connect" << std::endl;
    // waiting for players to connect

    while ((int)connectedUsers.size() != (int)maxPlayers) {};

    printf("Accept\n");
    std::cout << "cliente " << clientSockets.size() << std::endl;
    for (vector<int>::iterator it = clientSockets.begin(); it  != clientSockets.end(); it++) {
        std::cout << "cliente " << *it << std::endl;
    };
    

    startGame(config);

    for (int clientSocket : clientSockets) close(clientSocket);
    close(serverSocket);
    return 0;
}

// LOGIN
void * Server::connectToClient (void* arguments) {
    std::cout << "connecting" << std::endl;
    int socket = ((connectToClientArgs_t*)arguments)->serverSocket;
    std::vector<int>* clientSockets = ((connectToClientArgs_t*)arguments)->clientSockets;
    std::vector<user_t> users = ((connectToClientArgs_t*)arguments)->users;
    struct sockaddr_in* clientAddress = ((connectToClientArgs_t*)arguments)->clientAddress;
    int* clientAddrLen = ((connectToClientArgs_t*)arguments)->clientAddrLen;
    Server* server = ((connectToClientArgs_t*)arguments)->server;
    int client;
    accept:
        do {
            client = accept(socket, (struct sockaddr *)clientAddress, (socklen_t*) clientAddrLen);
        } while(client < 0);
    login:
        int result = server->startLogin(client, users);
        if (result == LOGIN_OK) goto connected;
        else if (result == LOGIN_ABORTED) {
            close(client);
            goto accept;
        }
        else goto login;
    connected:
        std::cout << "new player" << std::endl;
        (*clientSockets).push_back(client);
        return NULL;
}

bool Server::userAlreadyConnected(user_t user) {
    for (vector<user_t>::iterator it = connectedUsers.begin(); it  != connectedUsers.end(); it++) {
        user_t connected_user = *it ;
        if ( strcmp(connected_user.username ,user.username) == 0 && strcmp(connected_user.password , user.password) == 0) return true;
    }
    return false;
}

int Server::checkUser(user_t player, vector<user_t> users) {
    for (vector<user_t>::iterator it = users.begin(); it  != users.end(); it++) {
        user_t user = *it ;
        if ( strcmp(player.username ,user.username) == 0 && strcmp(player.password , user.password) == 0) {
            if(userAlreadyConnected(user)) {
                return USER_ALREADY_CONNECTED;
            }else {
                connectedUsers.push_back(user);
                return LOGIN_OK;
            }
        }
    }

    return INVALID_USER_PASS;
}

int Server::startLogin(int client, vector<user_t> users) {
    std::cout << "login" << std::endl;
    user_t player;
    int bytesReceived = receiveLogin(client, &player);
    std::cout << "bytes received: " << bytesReceived << std::endl;
    
    int response = checkUser(player, users);
    sendLoginResponse(client, &response);
    std::cout << "checked: " << response << std::endl;
    return response;
}


int Server::receiveLogin (int client, user_t* player) {
    int totalBytesReceived = 0;
    int bytesReceived = 0;
    int dataSize = sizeof(user_t);
    std::cout << "data size to receive: " << dataSize << std::endl;
    bool clientSocketStillOpen = true;
    
    while((totalBytesReceived < dataSize) && clientSocketStillOpen) {
        bytesReceived = recv(client, (player + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
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

int Server::sendLoginResponse (int client, int* response) {
    int totalBytesSent = 0;
    int bytesSent = 0;
    int dataSize = sizeof(int);
    bool clientSocketStillOpen = true;
    
    while((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = send(client, (response + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
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

// GAME

void Server::startGame(configuration::GameConfiguration config) {
    
    
    srand(time(NULL));
    SDL_Init(SDL_INIT_TIMER);

    std::vector<Mario*> players;
    for(unsigned int i = 0; i < (unsigned int)maxPlayers; ++i) {
        players.push_back(new Mario());
    }

    Uint8 currentLevel = 0;
    Nivel *nivel = NULL;

    getNextLevel(&nivel, &config, ++currentLevel);
    nivel->addPlayers(&players);

    handleCommandArgs_t handleCommandArgs[maxPlayers];
    for(unsigned int i = 0; i < clientSockets.size(); ++i) {
        handleCommandArgs[i].clientSocket = clientSockets[i];
        handleCommandArgs[i].mario = players[i];

        pthread_t recvCommandThread;
        pthread_create(&recvCommandThread, NULL, handleCommand, &handleCommandArgs[i]);
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
                getNextLevel(&nivel, &config, ++currentLevel);
                if (nivel == NULL) {
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
        if (bytesReceived == sizeof(controls_t)) player->setControls(controls);

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
        *nivel = NULL;
    }
}