#pragma once

#include "EntidadEstaticaVista.h"

class DonkeyKongVista : public EntidadEstaticaVista
{
public:
    DonkeyKongVista(SDL_Renderer *renderer);
    void mostrar();
    ~DonkeyKongVista()
    {
        SDL_DestroyTexture(texture);
    };

private:
    SDL_Texture *texture;
    SDL_Rect rect;
};