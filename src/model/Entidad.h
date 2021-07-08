#pragma once
#include <SDL2/SDL.h>
#include "../utils/punto.h"

class Entidad
{
public:
    Entidad(float posX, float posY, int ancho, int alto);
    virtual void mover() = 0; //separar entidades en entidades estaticas y dinamicas
    punto_t getPos();
    virtual ~Entidad() = default;
    virtual SDL_FRect dimensions() = 0;
    float posX;
    float posY;

protected:
    int ancho;
    int alto;
};