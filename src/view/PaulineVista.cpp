#include <string>
#include <SDL2/SDL_image.h>
#include "PaulineVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

const std::string IMG_PAULINE = "res/Pauline.png";

SDL_Texture *PaulineVista::texture = NULL;

PaulineVista::PaulineVista(SDL_Renderer* renderer)
: EntidadEstaticaVista() {
    this->renderer = renderer;
    SDL_Surface* surface = IMG_Load(IMG_PAULINE.c_str());
    if (surface == NULL) {
        logger::Logger::getInstance().logError("Image not found: " + IMG_PAULINE);
        logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
        surface = IMG_Load(IMG_DEFAULT.c_str());
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    srcRect = {0, 0, PAULINE_ANCHO, PAULINE_ALTO};

    dstRect.x = round(PAULINE_POS_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.y = round(PAULINE_POS_Y * ALTO_PANTALLA / (float)ALTO_NIVEL);
    dstRect.w = round(PAULINE_ANCHO * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(PAULINE_ALTO * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void PaulineVista::mostrar() {
    if (fin) {
        srcRect.x = 2 * 24;
    } else {
        ++counter;
        int animate = 32 <= counter && counter < 96;
        srcRect.x = animate * 24 * (1 - (counter / 8) % 2);
    }
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}