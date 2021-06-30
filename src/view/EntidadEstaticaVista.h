#pragma once

#include <SDL2/SDL.h>

class EntidadEstaticaVista
{
public:
    virtual void mostrar() = 0;
    virtual ~EntidadEstaticaVista() = default;

protected:
    static SDL_Renderer *renderer;
};