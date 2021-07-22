#include <string>
#include <SDL2/SDL_image.h>
#include "HammerVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define ANCHO_HAMMER 16
#define ALTO_HAMMER 16

const std::string IMG_HAMMER = "res/Hammer.png";

SDL_Renderer *HammerVista::renderer{nullptr};
SDL_Texture *HammerVista::texture{nullptr};

HammerVista::HammerVista(SDL_Renderer *renderer) {
    if (this->texture == nullptr) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_HAMMER.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_HAMMER);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
        this->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    srcRect = {0, 0, ANCHO_HAMMER, ALTO_HAMMER};

    dstRect.w = round(ANCHO_HAMMER * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(ALTO_HAMMER * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void HammerVista::setFlip(SDL_RendererFlip flip) {
    this->flip = flip;
}

void HammerVista::mostrar(punto_t pos, EstadoHammer estado) {
    srcRect.x = estado;
    dstRect.x = round(pos.x * (ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    dstRect.y = round(pos.y * (ALTO_PANTALLA / (float)ALTO_NIVEL));
    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0., NULL, flip);
}

HammerVista::~HammerVista() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}