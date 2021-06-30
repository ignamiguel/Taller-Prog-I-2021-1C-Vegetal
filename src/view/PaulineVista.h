#pragma once
#include "EntidadEstaticaVista.h"

class PaulineVista : public EntidadEstaticaVista
{
public:
    PaulineVista(SDL_Renderer *renderer);
    void mostrar();
    ~PaulineVista();

private:
    static SDL_Texture *texturePauline;
    static SDL_Texture *textureHelp;
    SDL_Rect srcRect, dstRect;
    SDL_Rect helpSrcRect, helpDstRect;
    unsigned char counter = 0;
    int fin = 0;
};