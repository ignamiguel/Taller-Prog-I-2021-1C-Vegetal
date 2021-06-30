#pragma once

#include "EntidadEstaticaVista.h"

class DefaultConfigVista : public EntidadEstaticaVista
{
public:
    DefaultConfigVista(SDL_Renderer *renderer);
    void mostrar();
    ~DefaultConfigVista()
    {
        SDL_DestroyTexture(texture);
    };

private:
    static SDL_Texture *texture;
    static SDL_Rect rect;
};