#pragma once

#include <SDL2/SDL.h>
#include "../utils/punto.h"

class PlataformaMovilVista
{
public:
    PlataformaMovilVista(SDL_Renderer *renderer);
    void mover(punto_t pos);
    void mostrar();
    ~PlataformaMovilVista();

private:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect dstRect;
};