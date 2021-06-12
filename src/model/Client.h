#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>

class Client {
    public:
    Client();
    int connectToServer(char* serverIp, char* port);
    private:
    void startGame();
    int receiveView(int* clientSocket, estadoNivel_t* view);
    int clientSocket;
    struct sockaddr_in serverAddress;
    
};