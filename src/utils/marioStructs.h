#pragma once
#include "punto.h"

enum SoundCode
{
    JUMP,
    DEATH,
    FINISHED_LEVEL,
    ITEM,
    ENEMY_DEATH
};

typedef struct sounds 
{
    unsigned char jump : 1;
    unsigned char death : 1;
    unsigned char finishedLevel : 1;
    unsigned char item : 1;
    unsigned char enemyDeath : 1;
    unsigned char : 3; //Espacio reservado para otros sonidos
} sounds_t;

typedef struct estadoMario
{
    punto_t pos;
    char estado;
    bool hasHammer;
    bool isEnabled;
    sounds_t sounds;
} estadoMario_t;

typedef struct controls
{
    unsigned char space : 1;
    unsigned char up : 1;
    unsigned char down : 1;
    unsigned char left : 1;
    unsigned char right : 1;
    unsigned char toggleTestMode : 1;
    unsigned char : 2;
} controls_t;

enum
{
    REPOSO,
    DE_ESPALDAS,
    CORRIENDO,
    SALTANDO,
    TREPANDO,
    MURIENDO,
    MUERTO,
    GAME_OVER
};

enum Direccion 
{
    DERECHA,
    IZQUIERDA
};