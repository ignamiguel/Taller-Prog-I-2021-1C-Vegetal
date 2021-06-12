#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include "../configuration.hpp"
#include "../logger.h"
#include "Nivel1.h"
#include "Nivel2.h"
#include "Mario.hpp"
#include "../view/Nivel1Vista.h"
#include "../view/Nivel2Vista.h"
#include "../controller/MarioController.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"
#include "../utils/estadoNivel.h"
#include "../utils/estadoMario.h"
#include "../utils/punto.h"
#include "Server.h"

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
    if (serverSocket == -1)
        return -1;

    this->serverSocket = serverSocket;

    //bind
    int serverBind = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if(serverBind < 0)
        return -1;

    //listen
    if (listen(serverSocket , MAX_QUEUED_CONNECTIONS) < 0)
        return -1;
    printf("listen...\n");
    //Accept
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t*) &clientAddrLen);
    if (clientSocket < 0)
        return -1;
    printf("accept\n");
    startGame();

    close(clientSocket);
    close(serverSocket);  
    return 0;
}

void Server::startGame() {
    logger::Logger::getInstance().logNewGame();
    
    auto configuration = configuration::GameConfiguration(CONFIG_FILE);
    auto log_level = configuration.getLogLevel();
    logger::Logger::getInstance().setLogLevel(log_level);

    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    //SDL_Window* window = SDL_CreateWindow(NOMBRE_JUEGO.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(NULL, -1, SDL_RENDERER_PRESENTVSYNC);

    Mario* mario = new Mario();
    //MarioController *marioController = new MarioController(mario);

    Uint8 currentLevel = 1;
    Nivel *nivel = NULL;
    NivelVista *vista = NULL;
    getNextLevel(&nivel, &vista, mario, &configuration, currentLevel, renderer);

    mario->setNivel(nivel);

    Uint32 previous, current, elapsed, lag;
    //bool updated, quitRequested = false;
    bool updated;
    previous = SDL_GetTicks();
    lag = 0;

    while(true) {
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

        // Update View and render
        if (updated) {
            estadoNivel_t* view = nivel->getEstado();
            int bytesSent = sendView(&(this->clientSocket), view);
            if(view == NULL)
                printf("null view \n");
            printf("sent %d/%d\n", bytesSent, (int)sizeof(estadoNivel_t));
        }
    }
}

int Server::sendView(int* clientSocket, estadoNivel_t* view) {
    int totalBytesSent = 0;
    int bytesSent = 0;
    int dataSize = sizeof(estadoNivel_t);
    bool clientSocketStillOpen = true;
    
    while((totalBytesSent < dataSize) && clientSocketStillOpen) {
        bytesSent = send(*clientSocket, (view + totalBytesSent), (dataSize - totalBytesSent), MSG_NOSIGNAL);
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

void Server::getNextLevel(Nivel **nivel, NivelVista **vista, Mario* mario, configuration::GameConfiguration *config, Uint8 currentLevel, SDL_Renderer *renderer) {
    if (currentLevel == 1) {
        logger::Logger::getInstance().logInformation("Level 1 starts");
        mario->setPos(N1_MARIO_POS_X, N1_MARIO_POS_Y);

        *nivel = new Nivel1();
        (*nivel)->addPlayer(mario);

        auto enemies = config->getEnemies();
        for (auto enemy: enemies) {
            if (enemy.getType().compare("Fuego") == 0) (*nivel)->addEnemies(enemy.getQuantity());
            logger::Logger::getInstance().logDebug("Enemy type: " + enemy.getType());
            logger::Logger::getInstance().logDebug("Enemy quantity: " + std::to_string(enemy.getQuantity()));
        }

        *vista = new Nivel1Vista(renderer, config->getDefaultConfigFlag());
        (*vista)->addPlayers(1);                                           // Aca iria cantidad de jugadores
        auto stages = config->getStages();
        if (stages.size() > 0) {
            std::string rutaImagen = stages[0].getBackgrounds()[0];
            logger::Logger::getInstance().logDebug("Stage 1 background img: " + rutaImagen);
            (*vista)->setBackground(rutaImagen);
        }
        return;
    }
    if (currentLevel == 2) {
        logger::Logger::getInstance().logInformation("Level 2 starts");
        mario->setPos(N2_MARIO_POS_X, N2_MARIO_POS_Y);

        delete *nivel;
        *nivel = new Nivel2();
        (*nivel)->addPlayer(mario);
        mario->setNivel((*nivel));

        delete *vista;
        *vista = new Nivel2Vista(renderer, config->getDefaultConfigFlag());
        (*vista)->addPlayers(1);                                           // Aca iria cantidad de jugadores
        auto stages = config->getStages();
        if (stages.size() > 1) {
            std::string rutaImagen = stages[1].getBackgrounds()[0];
            logger::Logger::getInstance().logDebug("Stage 2 background img: " + rutaImagen);
            (*vista)->setBackground(rutaImagen);
        }
        return;
    } else {
        delete *nivel;
        delete *vista;
        delete mario;
        *nivel = NULL;
    }
}