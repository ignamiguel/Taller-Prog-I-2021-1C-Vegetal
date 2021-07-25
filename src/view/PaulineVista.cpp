#include <string>
#include <SDL2/SDL_image.h>
#include "PaulineVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

const std::string IMG_PAULINE = "res/Pauline.png";

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

    rect.x = round(PAULINE_POS_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    rect.y = round(PAULINE_POS_Y * ALTO_PANTALLA / (float)ALTO_NIVEL);
    rect.w = round(PAULINE_ANCHO * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    rect.h = round(PAULINE_ALTO * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void PaulineVista::mostrar() {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}