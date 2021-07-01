#pragma once

#include <SDL2/SDL.h>
#include <arpa/inet.h>
#include "../utils/user.h"

class Client
{
public:
    Client(char *serverIp, char *port);
    int startClient();
    int showStartPage();
    int connectToServer();
    void startGame();
    char login(user_t user);

private:
    char *serverIp;
    char *port;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int clientSocket;
    struct sockaddr_in serverAddress;
    void showConnectedPage();
};