#pragma once
#include <SDL2/SDL.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../utils/estadoNivel.h"
#include "../utils/user.h"

class Client {
    public:
        Client();
        int startClient(char* serverIp, char* port);

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        int connectToServer(char* serverIp, char* port);
        void informConnectionFailure();

        bool login();
        int requireLogin (user_t* player);
        int receiveLoginResponse (int* response);

        void showConnectedPage();
        void startGame();
        static void *sendDataThread(void* args);
        static int sendCommand(int clientSocket, controls_t* command);
        static void *receiveDataThread(void *args);
        static int receiveView(int clientSocket, estadoNivel_t* view);
        
        int clientSocket;
        struct sockaddr_in serverAddress;
        user_t user;
};