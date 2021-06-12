#pragma once
#include <list>
#include "../utils/estadoMario.h"
#include "../utils/punto.h"

struct estadoNivel {
        punto_t platforms[12];
        punto_t enemies[3];
        punto_t barrels[20];
        estadoMario_t players[1];
        /*
        std::list<punto_t> platforms;
        std::list<punto_t> enemies;
        std::list<punto_t> barrels;
        std::list<estadoMario_t> players;
        */
};

typedef struct estadoNivel estadoNivel_t;
