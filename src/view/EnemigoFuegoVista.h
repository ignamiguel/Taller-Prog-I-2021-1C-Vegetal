#pragma once

#include <SDL2/SDL.h>
#include "../utils/punto.h"

class EnemigoFuegoVista
{
public:
    EnemigoFuegoVista(SDL_Renderer *renderer);
    void startRender();
    void mover(punto_t pos);
    void mostrar();
    ~EnemigoFuegoVista();

private:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    int tiempo;
    int updated;
};