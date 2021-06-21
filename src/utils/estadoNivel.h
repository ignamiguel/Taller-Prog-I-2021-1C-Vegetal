#pragma once
#include "marioStructs.h"
#include "punto.h"
#include "../utils/Constants.hpp"


struct estadoNivel {
        punto_t platforms[MAX_PLATFORMS];
        punto_t enemies[MAX_ENEMIES];
        punto_t barrels[MAX_BARRELS];
        estadoMario_t players[MAX_PLAYERS];
        unsigned char level;
};

typedef struct estadoNivel estadoNivel_t;
