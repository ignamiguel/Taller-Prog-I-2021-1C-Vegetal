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
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    SDL_Rect dstRect;
    static int totalPlataformas;
};