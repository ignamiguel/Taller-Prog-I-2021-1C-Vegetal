#pragma once
#include "marioStructs.h"
#include "punto.h"
#include "../utils/Constants.hpp"

typedef struct estadoJugador 
{
    char name[4];
    char lives;
    unsigned char score;
} estadoJugador_t;

struct estadoNivel
{
    punto_t platforms[MAX_PLATFORMS];
    punto_t enemies[MAX_ENEMIES];
    punto_t barrels[MAX_BARRELS];
    estadoMario_t players[MAX_PLAYERS];
    unsigned char level;
};

typedef struct estadoNivel estadoNivel_t;

typedef struct estadoJuego 
{
    estadoJugador_t players[MAX_PLAYERS];
    estadoNivel_t estadoNivel;
} estadoJuego_t;