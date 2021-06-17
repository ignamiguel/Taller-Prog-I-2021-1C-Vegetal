#pragma once
#include "EntidadEstaticaVista.h"

class PaulineVista : public EntidadEstaticaVista {
    public:
        PaulineVista(SDL_Renderer* renderer);
        void mostrar();
        ~PaulineVista() {
            SDL_DestroyTexture(texture);
        };

    private:
        static SDL_Texture* texture;
        SDL_Rect srcRect, dstRect;
        unsigned char counter = 0;
        bool fin = false;
};