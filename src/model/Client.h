#pragma once
#include <SDL2/SDL.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../utils/estadoNivel.h"

class Client {
    public:
    Client();
    void showWaitingView();
    int connectToServer(char* serverIp, char* port);

    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    void startGame();
    static void *sendDataThread(void* args);
    static int sendCommand(int clientSocket, controls_t* command);
    static void *recieveDataThread(void *args);
    static int receiveView(int clientSocket, estadoNivel_t* view);
    int clientSocket;
    struct sockaddr_in serverAddress;
};