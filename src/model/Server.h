#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "../utils/estadoNivel.h"
#include "../utils/user.h"
#include "../configuration.hpp"

class Server {
    public:
    Server(char* port);
    int startServer();
    
    private:
    int maxPlayers;
    std::vector<int> clientSockets;

    int startLogin(int, configuration::GameConfiguration);
    int receiveLogin (int,user_t*);
    int sendLoginResponse (int,int*);
    int checkUser(user_t, configuration::GameConfiguration);

    void startGame(configuration::GameConfiguration config);
    static int sendView(int clientSocket, estadoNivel_t* view);
    static int receiveCommand(int clientSocket, controls_t* controls);
    static void* handleCommand(void* handleCommandArgs);

    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int clientAddrLen;
};