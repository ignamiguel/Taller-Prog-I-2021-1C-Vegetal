#pragma once
#include "../utils/punto.h"
#include <SDL2/SDL.h>

class Entidad {
    public:
        Entidad(float posX, float posY, int ancho, int alto);
        virtual void mover() = 0; //separar entidades en entidades estaticas y dinamicas
        punto_t getPos();
        virtual ~Entidad() = default;
        SDL_FRect dimensions();

    protected:
        float posX;
        float posY;
        int ancho;
        int alto;

};