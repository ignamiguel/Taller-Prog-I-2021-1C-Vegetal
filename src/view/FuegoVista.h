#pragma once

#include "EntidadEstaticaVista.h"

class FuegoVista : public EntidadEstaticaVista
{
public:
    FuegoVista(int x, int y, SDL_Renderer *renderer);
    void mostrar();
    ~FuegoVista();

private:
    static SDL_Texture *texture;
    static SDL_Rect srcRect;
    SDL_Rect dstRect;
    int tiempo;
    static int totalFuegos;
};