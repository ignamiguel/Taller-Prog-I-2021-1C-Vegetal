#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unistd.h>

#include "Server.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "Nivel1.h"
#include "Nivel2.h"
#include "Mario.hpp"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/Messages.h"
#include "../utils/window.hpp"
#include "../utils/estadoNivel.h"
#include "../utils/player.h"

#define MAX_QUEUED_CONNECTIONS 3

typedef struct handleLoginArgs {
    Server *server;
    int clientSocket;
} handleLoginArgs_t;

void getNextLevel(Nivel **nivel, Uint8 currentLevel);

void *acceptNewConnections(void *serverArg);
void *handleLogin(void *arguments);
int validateUserLogin(int client, Server *server);
ssize_t receiveLoginRequest(int client, user_t *user);
ssize_t sendLoginResponse(int client, int *response);

void *handleCommand(void *player);
int receiveCommand(int clientSocket, controls_t *controls);
ssize_t sendView(int clientSocket, estadoNivel_t *view);

pthread_mutex_t connectedPlayersMutex;

Server::Server(char *port) {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(atoi(port));

    std::cout << "Aplicación iniciada en modo servidor en el puerto: " << port << '\n';

    logger::Logger::getInstance().logInformation("Loading valid users...");
    auto config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    for (auto u: config->getUsers())
    {
        this->users[u.username] = u;
        logger::Logger::getInstance().logDebug(std::string("user: ") + u.username + " " + u.password);
    }
}

int Server::startServer() {
    auto config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    auto logLevel = config->getLogLevel();
    logger::Logger::getInstance().setLogLevel(logLevel);

    this->maxPlayers = config->getMaxPlayers();
    if(this->maxPlayers < 1) {
        logger::Logger::getInstance().logDebug(CANTIDAD_DE_JUGADORES_INVALIDA);
        this->maxPlayers = DEFAULT_MAX_PLAYERS;
    }

    //socket
    serverSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (serverSocket == -1) {
        return EXIT_FAILURE;
    }

    //bind
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        return EXIT_FAILURE;
    }

    //listen
    if (listen(serverSocket , MAX_QUEUED_CONNECTIONS) < 0) {
        return EXIT_FAILURE;
    }

    logger::Logger::getInstance().logInformation("Waiting for players...");

    pthread_t acceptConnectionsThread;
    pthread_create(&acceptConnectionsThread, NULL, acceptNewConnections, this);

    while (this->connectedPlayers.size() < (unsigned int)maxPlayers) {}

    startGame();

    for (auto player : connectedPlayers) {
        close(player.second->clientSocket);
    }

    close(serverSocket);
    return EXIT_SUCCESS;
}

void Server::startGame() {
    logger::Logger::getInstance().logNewGame();
    
    srand(time(NULL));
    SDL_Init(SDL_INIT_TIMER);

    std::vector<Mario *> marios;
    for(unsigned int i = 0; i < (unsigned int)maxPlayers; ++i) {
        marios.push_back(new Mario());
    }

    Uint8 currentLevel = 0;
    Nivel *nivel = NULL;

    getNextLevel(&nivel, ++currentLevel);
    nivel->addPlayers(&marios);

    {
        size_t i = 0;
        for(auto it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
            it->second->mario = marios[i++];
        }
    }

    Uint32 previous, current, elapsed, lag;
    bool updated, quitRequested = false;
    previous = SDL_GetTicks();
    lag = 0;
    while (!quitRequested) {
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
            for(auto it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                if (it->second->isConnected) {
                    it->second->isConnected = sendView(it->second->clientSocket, view) == sizeof(estadoNivel_t);
                }
            }
            if (__builtin_expect(nivel->isComplete(), 0)) {
                getNextLevel(&nivel, ++currentLevel);
                if (nivel == NULL) {
                    break;
                }
                nivel->addPlayers(&marios);
            }
        }
        quitRequested = SDL_QuitRequested();
    }
}

// START LOGIN
void *acceptNewConnections(void* serverArg) {
    Server* server = (Server *)serverArg;

    while(true) {
        int client = accept(server->serverSocket, (struct sockaddr *)&(server->clientAddress), (socklen_t*) &(server->clientAddrLen));

        handleLoginArgs_t arguments;
        arguments.clientSocket = client;
        arguments.server = server;

        pthread_t clientConnection;
        pthread_create(&clientConnection, NULL, handleLogin, &arguments);
    }
}

void *handleLogin(void* arguments) {
    Server *server = ((handleLoginArgs_t*)arguments)->server;
    int client = ((handleLoginArgs_t*)arguments)->clientSocket;

    int response;

    do {
        response = validateUserLogin(client, server);
    } while (response != LOGIN_OK && response != LOGIN_ABORTED);

    if (response == LOGIN_ABORTED) {
        close(client);
    }

    return NULL;
}

int validateUserLogin(int client, Server *server) {
    user_t user;
    if (receiveLoginRequest(client, &user) != sizeof(user_t)) {
        logger::Logger::getInstance().logDebug("Lost connection to client");
        return LOGIN_ABORTED;
    }

    int response = -1;

    if (server->users.count(user.username) == 0) {
        logger::Logger::getInstance().logDebug(std::string("[") + user.username + "] invalid user");
        response = LOGIN_INVALID_USER;
        sendLoginResponse(client, &response);
        return LOGIN_INVALID_USER;
    }

    if (strcmp(server->users.at(user.username).password, user.password) != 0) {
        logger::Logger::getInstance().logDebug(std::string("[") + user.username + "] incorrect password");
        response = LOGIN_INVALID_USER_PASS;
        sendLoginResponse(client, &response);
        return LOGIN_INVALID_USER_PASS;
    }

    pthread_mutex_lock(&connectedPlayersMutex);
    if (server->connectedPlayers.count(user.username) != 0) {
        player_t *player = server->connectedPlayers[user.username];
        if (player->isConnected) {
            pthread_mutex_unlock(&connectedPlayersMutex);
            logger::Logger::getInstance().logDebug(std::string("[") + user.username + "] user already connected");
            response = LOGIN_USER_ALREADY_CONNECTED;
            sendLoginResponse(client, &response);
            return LOGIN_USER_ALREADY_CONNECTED;
        }
        else {
            close(player->clientSocket);
            player->clientSocket = client;
            response = LOGIN_OK;
            player->isConnected = true;
            logger::Logger::getInstance().logInformation(std::string("Succesfully reconnected ") + user.username);
        }
    }

    if (server->connectedPlayers.size() == server->maxPlayers && response == -1) {
        pthread_mutex_unlock(&connectedPlayersMutex);
        response = LOGIN_MAX_USERS_CONNECTED;
        sendLoginResponse(client, &response);
        return LOGIN_MAX_USERS_CONNECTED;
    }

    // Usuario valido: login OK
    // Lo agrego a jugadores conectados
    if (response == -1) {
        response = LOGIN_OK;
        player_t *newPlayer = new player_t();
        newPlayer->clientSocket = client;
        newPlayer->isConnected = true;
        newPlayer->mario = NULL;
        newPlayer->user = user;
        server->connectedPlayers[user.username] = newPlayer;
        logger::Logger::getInstance().logInformation(std::string("Accepted new user: ") + user.username);
    }
    pthread_mutex_unlock(&connectedPlayersMutex);

    pthread_t recvCommandThread;
    pthread_create(&recvCommandThread, NULL, handleCommand, server->connectedPlayers[user.username]);

    sendLoginResponse(client, &response);
    return LOGIN_OK;
}

ssize_t receiveLoginRequest(int client, user_t *user) {
    size_t totalBytesReceived = 0;
    ssize_t bytesReceived = 0;
    size_t dataSize = sizeof(user_t);
    bool clientSocketStillOpen = true;

    while ((totalBytesReceived < dataSize) && clientSocketStillOpen) {
        bytesReceived = recv(client, (user + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
        if (bytesReceived < 0) {
            return bytesReceived;
        }
        else if (bytesReceived == 0) {
            clientSocketStillOpen = false;
        }
        else {
            totalBytesReceived += bytesReceived;
        }
    }
    return totalBytesReceived;
}

ssize_t sendLoginResponse(int client, int *response) {
    size_t totalBytesSent = 0;
    ssize_t bytesSent = 0;
    size_t dataSize = sizeof(int);
    bool clientSocketStillOpen = true;

    while ((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = send(client, (response + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
        if (bytesSent < 0) {
            return bytesSent;
        }
        else if (bytesSent == 0) {
            clientSocketStillOpen = false;
        }
        else {
            totalBytesSent += bytesSent;
        }
    }
    return totalBytesSent;
}
// END LOGIN

void *handleCommand(void *player) {
    Mario *mario;
    while ((mario = ((player_t *)player)->mario) == NULL) {}
    mario->enable();

    int clientSocket = ((player_t *)player)->clientSocket;
    controls_t controls;

    bool quitRequested = false;
    while (!quitRequested) {
        if (receiveCommand(clientSocket, &controls) == sizeof(controls_t)) {
            mario->setControls(controls);
        } else {
            break;
        }
        quitRequested = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_QUIT, SDL_QUIT) > 0;
    }
    mario->disable();
    mario->setControls({0, 0, 0, 0, 0});
    shutdown(clientSocket, SHUT_RD);
    return NULL;
}

int receiveCommand(int clientSocket, controls_t *controls) {
    size_t totalBytesSent = 0;
    ssize_t bytesSent = 0;
    size_t dataSize = sizeof(controls_t);
    bool clientSocketStillOpen = true;
    
    while ((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = recv(clientSocket, (controls + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
        if (bytesSent < 0) {
            return bytesSent;
        }
        else if (bytesSent == 0) {
            clientSocketStillOpen = false;
        }
        else {
            totalBytesSent += bytesSent;
        }
    }

    return totalBytesSent;
}

ssize_t sendView(int clientSocket, estadoNivel_t* view) {
    size_t totalBytesSent = 0;
    ssize_t bytesSent = 0;
    size_t dataSize = sizeof(estadoNivel_t);
    bool clientSocketStillOpen = true;
    
    while ((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = send(clientSocket, (view + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
        if (bytesSent < 0) {
            return bytesSent;
        }
        else if (bytesSent == 0) {
            clientSocketStillOpen = false;
        }
        else {
            totalBytesSent += bytesSent;
        }
    }

    return totalBytesSent;
}

void getNextLevel(Nivel **nivel, Uint8 currentLevel) {
    delete *nivel;
    if (currentLevel == 1) {
        logger::Logger::getInstance().logInformation("Level 1 starts");
        *nivel = new Nivel1();
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