#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "../utils/estadoJuego.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

class StatsVista
{
public:
    StatsVista(SDL_Renderer *renderer);
    ~StatsVista() { SDL_DestroyTexture(iconsTexture); iconsTexture = nullptr; }

    void mostrar(const estadoJugador_t &estado, size_t nroJugador);

private:
    static SDL_Renderer *renderer;
    static SDL_Texture *iconsTexture;
    SDL_Rect srcRect{0, 0, 7, 8};
    SDL_Rect dstRect{0,
                     int(4 * ALTO_PANTALLA / (float)ALTO_NIVEL),
                     int(7 * ANCHO_PANTALLA / (float)ALTO_NIVEL),
                     int(8 * ALTO_PANTALLA / (float)ALTO_NIVEL)};
    std::vector<float> posiciones;
};