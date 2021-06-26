#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include "../utils/estadoNivel.h"
#include "../configuration.hpp"
#include <queue>
#include <map>

class Server {
    public:
        Server(char* port);
        int startServer();
    
    private:
        int maxPlayers;
        int serverSocket;
        std::vector<int> clientSockets;
        void startGame(configuration::GameConfiguration config);
        static int sendView(int clientSocket, estadoNivel_t* view);
        static int receiveCommand(int clientSocket, controls_t* controls);
        static void* handleCommand(void* handleCommandArgs);
        static void* acceptNewConnections(void* server);
        struct sockaddr_in serverAddress;
        struct sockaddr_in clientAddress;
        int clientAddrLen;
        std::queue<int> clientSocketQueue;
        std::map<std::string, user_t> users;
        std::map<std::string, user_t> connected_users;
        std::vector<user_t> connectedUsers;
        
        static void* connectToClient(void* arguments);
        int startLogin(int client);
        int receiveLogin (int client, user_t* player);
        int sendLoginResponse (int client, int* response);
};