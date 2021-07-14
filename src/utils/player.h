#pragma once
#include "../model/Mario.hpp"

typedef struct player
{
    char user[4];
    Mario *mario;
    int clientSocket;
    bool isConnected;
} player_t;