#pragma once

#include "../model/Mario.hpp"
#include "user.h"

typedef struct player
{
    user_t user;
    Mario *mario;
    int clientSocket;
    bool isConnected;
} player_t;