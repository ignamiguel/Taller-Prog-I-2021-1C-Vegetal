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
    void mostrar(int x, int y, EstadoHammer estado, SDL_RendererFlip flip);
    void mostrar(punto_t pos);
    ~HammerVista();

private:
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
};