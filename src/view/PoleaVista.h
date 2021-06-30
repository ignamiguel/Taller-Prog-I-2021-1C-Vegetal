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
    SDL_RendererFlip flip;
    static SDL_Rect rectSrc;
    SDL_Rect rectDst;
    int tiempo;
    static int totalPoleas;
};