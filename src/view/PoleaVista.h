#pragma once

#include "EntidadEstaticaVista.h"

class PoleaVista : public EntidadEstaticaVista
{
public:
    PoleaVista(int x, int y, int orientation, int rotation, SDL_Renderer *renderer);
    void mostrar();
    ~PoleaVista();

private:
    static SDL_Texture *texture;
    static SDL_Rect rectSrc;
    SDL_Rect rectDst;
    SDL_RendererFlip flip;
    int tiempo;
    static int totalPoleas;
};