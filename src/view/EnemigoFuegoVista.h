#pragma once

#include <SDL2/SDL.h>
#include "../utils/punto.h"

class EnemigoFuegoVista {
    public:
        EnemigoFuegoVista(SDL_Renderer *renderer);
        void startRender();
        void mover(punto_t pos);
        void mostrar();
        ~EnemigoFuegoVista();

    private:
        static SDL_Renderer *renderer;
        static SDL_Texture *texture;
        static SDL_Rect srcRect;
        static SDL_Rect dstRect;
        int tiempo;
        int updated;
};