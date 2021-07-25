#include <string>
#include <SDL2/SDL_image.h>
#include "BarrilVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define TIEMPO_POR_FRAME 4
#define CANT_FRAMES 4
#define SPRITE_INDEX_SIZE 24

const std::string IMG_BARRIL = "res/Barril.png";

BarrilVista::BarrilVista(SDL_Renderer *renderer) : srcRect{0, 0, ANCHO_BARRIL, ALTO_BARRIL} {
    this->renderer = renderer;
    SDL_Surface* surface = IMG_Load(IMG_BARRIL.c_str());
    if (surface == NULL) {
        logger::Logger::getInstance().logError("Image not found: " + IMG_BARRIL);
        logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
        surface = IMG_Load(IMG_DEFAULT.c_str());
    } else SDL_SetColorKey(surface, SDL_TRUE, *(Uint32*)(surface->pixels));
    this->texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    dstRect.w = round(ANCHO_BARRIL * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(ALTO_BARRIL * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void BarrilVista::startRender() {
    updated = 0;
}

void BarrilVista::mover(punto_t pos) {
    dstRect.x = round(pos.x * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.y = round(pos.y * ALTO_PANTALLA / (float)ALTO_NIVEL);
    flip = (110 - ALTO_BARRIL < pos.y && pos.y < 142.75 - ALTO_BARRIL)
        || (175.5 - ALTO_BARRIL < pos.y && pos.y < 208.25 - ALTO_BARRIL) ?
        SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

void BarrilVista::mostrar() {
    tiempo = (tiempo + (updated == 0)) % (TIEMPO_POR_FRAME * CANT_FRAMES);
    srcRect.x = (tiempo / TIEMPO_POR_FRAME) * SPRITE_INDEX_SIZE;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0., NULL, flip);
    updated = 1;
}

BarrilVista::~BarrilVista() {
    SDL_DestroyTexture(texture);
}