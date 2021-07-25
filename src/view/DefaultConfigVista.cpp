#include <string>
#include <SDL2/SDL_image.h>
#include "DefaultConfigVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

const std::string IMG_DEFAULT_CONFIG = "res/default-config.png";

DefaultConfigVista::DefaultConfigVista(SDL_Renderer *renderer)
: EntidadEstaticaVista() {
    this->renderer = renderer;
    SDL_Surface* surface = IMG_Load(IMG_DEFAULT_CONFIG.c_str());
    if(surface == NULL) {
        logger::Logger::getInstance().logError("Image not found:" + IMG_DEFAULT_CONFIG);
        logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
        surface = IMG_Load(IMG_DEFAULT.c_str());
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    rect.x = round(DEFAULT_CONFIG_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    rect.y = round(DEFAULT_CONFIG_Y * ALTO_PANTALLA / (float)ALTO_NIVEL);
    rect.w = round(ANCHO_DEFAULT_CONFIG * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    rect.h = round(ALTO_DEFAULT_CONFIG * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void DefaultConfigVista::mostrar() {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}