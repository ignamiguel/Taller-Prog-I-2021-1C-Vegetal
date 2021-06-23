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

        static void* connectToClient (void*);
        int startLogin(int, std::vector<user_t>);
        int checkUser(user_t, std::vector<user_t>);
        bool userAlreadyConnected(user_t);
        int receiveLogin (int,user_t*);
        int sendLoginResponse (int,int*);

        void startGame(configuration::GameConfiguration config);
        static int sendView(int clientSocket, estadoNivel_t* view);
        static int receiveCommand(int clientSocket, controls_t* controls);
        static void* handleCommand(void* handleCommandArgs);
	
	    std::vector<user_t> connectedUsers;
        struct sockaddr_in serverAddress;
        struct sockaddr_in clientAddress;
        int clientAddrLen;
};
