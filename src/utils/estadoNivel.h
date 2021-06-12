#pragma once
#include <list>
#include "../utils/estadoMario.h"
#include "../utils/punto.h"

const int MAX_PLATFORMS = 12;
const int MAX_ENEMIES = 3;
const int MAX_BARRELS = 20;
const int MAX_PLAYERS = 1;

struct estadoNivel {
        punto_t platforms[MAX_PLATFORMS];
        punto_t enemies[MAX_ENEMIES];
        punto_t barrels[MAX_BARRELS];
        estadoMario_t players[MAX_PLAYERS];
};

typedef struct estadoNivel estadoNivel_t;
