#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Nivel1.h"
#include "Nivel2.h"
#include "../view/Nivel1Vista.h"
#include "../view/Nivel2Vista.h"
#include "Mario.hpp"
#include "../controller/MarioController.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../utils/window.hpp"
#include "../utils/Constants.hpp"
#include "Server.h"
#include "Client.h"

Client::Client() {
    std::cout << "Aplicación iniciada en modo cliente" << std::endl;
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
    
    std::cout << "post game" << std::endl;
    return 1;
}

void Client::startGame() {
    logger::Logger::getInstance().logNewGame();
    
    auto configuration = configuration::GameConfiguration(CONFIG_FILE);
    auto log_level = configuration.getLogLevel();
    logger::Logger::getInstance().setLogLevel(log_level);

    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(NOMBRE_JUEGO.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    //SDL_Event event;


    //Uint8 currentLevel = 1;
    //Nivel *nivel = NULL;
    NivelVista *vista = NULL;
    
    //getNextLevel(&nivel, &vista, mario, &configuration, currentLevel, renderer);
    vista = new Nivel1Vista(renderer, configuration.getDefaultConfigFlag());
    vista->addPlayers(1);                                           // Aca iria cantidad de jugadores
    auto stages = configuration.getStages();
    if (stages.size() > 0) {
        std::string rutaImagen = stages[0].getBackgrounds()[0];
        logger::Logger::getInstance().logDebug("Stage 1 background img: " + rutaImagen);
        vista->setBackground(rutaImagen);
    }

    // Game loop design by http://gameprogrammingpatterns.com/game-loop.html#play-catch-up
    //Uint32 previous, current, elapsed, lag;
    //bool updated, quitRequested = false;
    bool quitRequested = false;
    //previous = SDL_GetTicks();
    //lag = 0;

    while (!quitRequested) {
        estadoNivel_t view;
        int bytesReceived = receiveView(&clientSocket, &view);
        printf("received %d/%d\n", bytesReceived, (int)sizeof(estadoNivel_t));
        if(bytesReceived == sizeof(estadoNivel_t)) {
            SDL_RenderClear(renderer);
            vista->update(&view);
            SDL_RenderPresent(renderer);
        }
        // Handle quit request
        quitRequested = SDL_QuitRequested();
    }
    logger::Logger::getInstance().logInformation("Game over");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

}

int Client::receiveView(int* clientSocket, estadoNivel_t* view) {
    
    int totalBytesReceived = 0;
    int bytesReceived = 0;
    int dataSize = sizeof(estadoNivel_t);
    bool clientSocketStillOpen = true;

    while((totalBytesReceived < dataSize) && clientSocketStillOpen) {
        bytesReceived = recv(*clientSocket, (view + totalBytesReceived), (dataSize - totalBytesReceived), MSG_NOSIGNAL);
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