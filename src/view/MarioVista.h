#pragma once

#include <SDL2/SDL.h>
#include "../utils/marioStructs.h"
#include "HammerVista.h"

class MarioVista
{
public:
    MarioVista(SDL_Renderer *renderer, HammerVista* hammer);
    MarioVista(const MarioVista &other);
    void mostrar(const estadoMario_t &estado);
    void setColor(size_t color);
    ~MarioVista();

private:
    static SDL_Renderer *renderer;
    static SDL_Texture *texture;
    static size_t totalJugadores;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    SDL_RendererFlip flip{SDL_FLIP_HORIZONTAL};
    int tiempo{0};
    int tiempoReposo{0};
    int tiempoCorriendoConMartillo{0};
    void updateReposo(char estado, bool hasHammer);
    void updateCorriendo(int nextX, bool hasHammer);
    void updateSaltando(int nextX);
    void updateTrepando(int nextY);
    void updateMuriendo(char estado);
    void drawHammer(int frame);
    HammerVista* vistaMartillo{nullptr};
};