#include <string>
#include <SDL2/SDL_image.h>
#include "BarrilVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define TIEMPO_POR_FRAME 3
#define CANT_FRAMES 4
#define SPRITE_INDEX_SIZE 24

const std::string IMG_BARRIL = "res/Barril.png";

SDL_Renderer *BarrilVista::renderer = NULL;
SDL_Texture *BarrilVista::texture = NULL;

BarrilVista::BarrilVista(SDL_Renderer *renderer) {
    if (this->texture == NULL) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_BARRIL.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_BARRIL);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        } else SDL_SetColorKey(surface, SDL_TRUE, *(Uint32*)(surface->pixels));
        this->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    srcRect = {0, 0, ANCHO_BARRIL, ALTO_BARRIL};

    dstRect.w = round(ANCHO_BARRIL * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(ALTO_BARRIL * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void BarrilVista::startRender() {
    updated = 0;
}

void BarrilVista::mover(punto_t pos) {
    dstRect.x = round(pos.x * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.y = round(pos.y * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void BarrilVista::mostrar() {
    tiempo = (tiempo + (updated == 0)) % (TIEMPO_POR_FRAME * CANT_FRAMES);
    srcRect.x = (tiempo / TIEMPO_POR_FRAME) * SPRITE_INDEX_SIZE;

    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    updated = 1;
}

BarrilVista::~BarrilVista() {
    SDL_DestroyTexture(texture);
    texture = NULL;
}