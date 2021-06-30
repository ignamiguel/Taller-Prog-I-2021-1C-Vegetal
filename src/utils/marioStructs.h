#pragma once
#include "punto.h"

typedef struct estadoMario
{
    punto_t pos;
    char estado;
} estadoMario_t;

typedef struct controls
{
    unsigned char space : 1;
    unsigned char up : 1;
    unsigned char down : 1;
    unsigned char left : 1;
    unsigned char right : 1;
    unsigned char : 3;
} controls_t;