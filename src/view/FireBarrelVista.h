#pragma once

#include "EntidadEstaticaVista.h"

class FireBarrelVista : public EntidadEstaticaVista
{
public:
    FireBarrelVista(SDL_Renderer *renderer);
    void mostrar();
    ~FireBarrelVista()
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    };

private:
    static SDL_Texture *texture;
    SDL_Rect rect;
};