#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <string.h>
#include <chrono>

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
#include "../utils/estadoJuego.h"
#include "../utils/player.h"
#include "../utils/dataTransfer.h"

#define MAX_QUEUED_CONNECTIONS 3
#define MS_PER_UPDATE std::chrono::milliseconds{17}

typedef struct handleLoginArgs {
    Server *server;
    int clientSocket;
} handleLoginArgs_t;

void getNextLevel(Nivel *&nivel, unsigned char currentLevel);
void getEstadoJugadores(estadoJuego_t &estado,  std::map<std::string, player_t> &connectedPlayers);

void *acceptNewConnections(void *serverArg);
void *handleLogin(void *arguments);
int validateUserLogin(int client, Server *server);

void *handleCommand(void *player);

pthread_mutex_t connectedPlayersMutex;

Server::Server(char *port) {
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(atoi(port));

    std::cout << "Aplicación iniciada en modo servidor en el puerto: " << port << '\n';

    logger::Logger::getInstance().logInformation("Loading valid users...");
    auto config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    for (auto &u : config->getUsers())
    {
        this->users[u.username] = u;
        logger::Logger::getInstance().logDebug(std::string("user: ") + u.username + " " + u.password);
    }
}

int Server::startServer() {
    auto config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    auto logLevel = config->getLogLevel();
    logger::Logger::getInstance().setLogLevel(logLevel);

    int configPlayers = config->getMaxPlayers();
    if(configPlayers < MIN_PLAYERS || MAX_PLAYERS < configPlayers) {
        logger::Logger::getInstance().logDebug(CANTIDAD_DE_JUGADORES_INVALIDA);
        maxPlayers = DEFAULT_MAX_PLAYERS;
    }
    else {
        maxPlayers = configPlayers;
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
    pthread_create(&acceptConnectionsThread, nullptr, acceptNewConnections, this);

    while (this->connectedPlayers.size() < maxPlayers) {}

    startGame();

    for (auto &player : connectedPlayers) {
        close(player.second.clientSocket);
    }

    close(serverSocket);
    return EXIT_SUCCESS;
}

void Server::startGame() {
    logger::Logger::getInstance().logNewGame();
    
    srand(time(NULL));

    std::vector<Mario> marios;
    for(size_t i = 0; i < maxPlayers; ++i) {
        marios.emplace_back();
    }

    unsigned char currentLevel = 0;
    Nivel *nivel{nullptr};
    estadoJuego_t game;

    getNextLevel(nivel, ++currentLevel);
    nivel->addPlayers(marios);

    {
        size_t i = 0;
        for(auto &player : connectedPlayers) {
            player.second.mario = &marios[i];
            strcpy(game.players[i++].name, player.second.user);
        }
    }

    std::chrono::time_point <std::chrono::steady_clock, std::chrono::milliseconds> previous, current;
    previous = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
    std::chrono::milliseconds elapsed;
    std::chrono::milliseconds lag{0};
    bool updated = false;
    while (nivel != nullptr) {
        current = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
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
            game.estadoNivel = nivel->getEstado();

            getEstadoJugadores(game, connectedPlayers);

            for(auto &player : connectedPlayers) {
                if (player.second.isConnected) {
                    player.second.isConnected = sendData(player.second.clientSocket, &game) == sizeof(estadoJuego_t);
                }
                player.second.mario->audioObserver.reset();
            }
            if (__builtin_expect(nivel->isComplete(), 0)) {
                getNextLevel(nivel, ++currentLevel);
                if (nivel != nullptr) {
                    nivel->addPlayers(marios);
                }
            }
        }
    }
}

// START LOGIN
void *acceptNewConnections(void* serverArg) {
    Server* server = (Server *)serverArg;

    while(true) {
        struct sockaddr_in clientAddress;
        int clientAddrLen;
        int client = accept(server->serverSocket, (struct sockaddr *)&clientAddress, (socklen_t*)&clientAddrLen);

        handleLoginArgs_t arguments;
        arguments.clientSocket = client;
        arguments.server = server;

        pthread_t clientConnection;
        pthread_create(&clientConnection, nullptr, handleLogin, &arguments);
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

    return nullptr;
}

int validateUserLogin(int client, Server *server) {
    user_t user;
    if (receiveData(client, &user) != sizeof(user_t)) {
        logger::Logger::getInstance().logDebug("Lost connection to client");
        return LOGIN_ABORTED;
    }

    char response = -1;

    if (server->users.count(user.username) == 0) {
        logger::Logger::getInstance().logDebug(std::string("[") + user.username + "] invalid user");
        response = LOGIN_INVALID_USER;
        sendData(client, &response);
        return LOGIN_INVALID_USER;
    }

    if (strcmp(server->users.at(user.username).password, user.password) != 0) {
        logger::Logger::getInstance().logDebug(std::string("[") + user.username + "] incorrect password");
        response = LOGIN_INVALID_USER_PASS;
        sendData(client, &response);
        return LOGIN_INVALID_USER_PASS;
    }

    pthread_mutex_lock(&connectedPlayersMutex);
    if (server->connectedPlayers.count(user.username) != 0) {
        player_t &player = server->connectedPlayers.at(user.username);
        if (player.isConnected) {
            pthread_mutex_unlock(&connectedPlayersMutex);
            logger::Logger::getInstance().logDebug(std::string("[") + user.username + "] user already connected");
            response = LOGIN_USER_ALREADY_CONNECTED;
            sendData(client, &response);
            return LOGIN_USER_ALREADY_CONNECTED;
        }
        else {
            close(player.clientSocket);
            player.clientSocket = client;
            response = LOGIN_OK;
            player.isConnected = true;
            logger::Logger::getInstance().logInformation(std::string("Succesfully reconnected ") + user.username);
        }
    }

    if (server->connectedPlayers.size() == server->maxPlayers && response == -1) {
        pthread_mutex_unlock(&connectedPlayersMutex);
        response = LOGIN_MAX_USERS_CONNECTED;
        sendData(client, &response);
        return LOGIN_MAX_USERS_CONNECTED;
    }

    // Usuario valido: login OK
    // Lo agrego a jugadores conectados
    if (response == -1) {
        response = LOGIN_OK;
        server->connectedPlayers[user.username] = {{user.username[0], user.username[1], user.username[2], '\0'},
                                                   nullptr,
                                                   client,
                                                   true};
        logger::Logger::getInstance().logInformation(std::string("Accepted new user: ") + user.username);
    }
    pthread_mutex_unlock(&connectedPlayersMutex);

    pthread_t recvCommandThread;
    pthread_create(&recvCommandThread, nullptr, handleCommand, &server->connectedPlayers[user.username]);

    sendData(client, &response);
    return LOGIN_OK;
}
// END LOGIN

void *handleCommand(void *player) {
    while (((player_t *)player)->mario == nullptr) {}

    int clientSocket = ((player_t *)player)->clientSocket;
    Mario &mario = *((player_t *)player)->mario;
    mario.enable();

    controls_t controls;

    bool clientOpen = true;
    while (clientOpen) {
        if (receiveData(clientSocket, &controls) == sizeof(controls_t)) {
            mario.controls = controls;
        } else {
            clientOpen = false;
        }
    }
    mario.disable();
    shutdown(clientSocket, SHUT_RD);
    return nullptr;
}

void getNextLevel(Nivel *&nivel, unsigned char currentLevel) {
    delete nivel;
    if (currentLevel == 1) {
        logger::Logger::getInstance().logInformation("Level 1 starts");
        nivel = new Nivel1();
    }
    else if (currentLevel == 2) {
        logger::Logger::getInstance().logInformation("End of Level 1");
        nivel = new Nivel2();
        logger::Logger::getInstance().logInformation("Level 2 starts");
    }
    else {
        logger::Logger::getInstance().logInformation("End of Level 2");
        nivel = nullptr;
    }
}

void getEstadoJugadores(estadoJuego_t &estado,  std::map<std::string, player_t> &connectedPlayers) {
    size_t i = 0;
    for (auto &player : connectedPlayers) {
        estado.players[i].lives = player.second.mario->lives;
        estado.players[i++].score = player.second.mario->score;
    }
}