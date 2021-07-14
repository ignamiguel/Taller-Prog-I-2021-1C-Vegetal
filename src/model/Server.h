#pragma once
#include <arpa/inet.h>
#include <map>
#include "../utils/player.h"
#include "../utils/user.h"

class Server
{
public:
    Server(char *port);
    int startServer();

    std::map<std::string, user_t> users;
    int serverSocket;
    size_t maxPlayers;
    std::map<std::string, player_t> connectedPlayers;

private:
    void startGame();

    struct sockaddr_in serverAddress;
};