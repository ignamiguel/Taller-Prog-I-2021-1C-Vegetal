#pragma once
#include "punto.h"
#include "marioStructs.h"
#include "exitStatus.h"
#include "Constants.hpp"

typedef struct estadoJugador
{
    char name[4];
    char lives;
    unsigned char score;
} estadoJugador_t;

typedef struct estadoNivel
{
    union {
        struct {
            punto_t platforms[MAX_PLATFORMS];
            punto_t enemies[MAX_ENEMIES];
        };
        struct {
            punto_t barrels[MAX_BARRELS];
        };
    };
    punto_t hammers[MAX_HAMMERS];
    estadoMario_t players[MAX_PLAYERS];
    unsigned char sceneNumber;
    bool gameOver{false};
} estadoNivel_t;

typedef struct estadoJuego
{
    estadoJugador_t players[MAX_PLAYERS];
    estadoNivel_t estadoNivel;
} estadoJuego_t;