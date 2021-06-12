#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>

class Server {
    public:
    Server(char* port);
    int startServer();
    int sendView(int* clientSocket, estadoNivel_t* view);
    
    private:
    void startGame();
    void getNextLevel(Nivel **nivel, NivelVista **vista, Mario* mario, configuration::GameConfiguration *config, Uint8 currentLevel, SDL_Renderer *renderer);

    int serverSocket;
    int clientSocket;

    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int clientAddrLen;
};