#pragma once

#include <SDL2/SDL.h>
#include "../utils/punto.h"

class BarrilVista
{
public:
    BarrilVista(SDL_Renderer *renderer);
    void startRender();
    void mover(punto_t pos);
    void mostrar();
    ~BarrilVista();

private:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    int tiempo{};
    int updated{};
    SDL_RendererFlip flip{SDL_FLIP_NONE};
};
