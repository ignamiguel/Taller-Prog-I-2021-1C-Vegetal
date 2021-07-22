#pragma once

#include <SDL2/SDL.h>
#include "../utils/punto.h"

enum EstadoHammer
{
    LEVANTADO,
    GOLPEANDO
};

class HammerVista
{
public:
    HammerVista(SDL_Renderer *renderer);
    void setFlip(SDL_RendererFlip flip);
    void mostrar(punto_t pos, EstadoHammer estado);
    ~HammerVista();

private:
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    SDL_RendererFlip flip{SDL_FLIP_NONE};
};