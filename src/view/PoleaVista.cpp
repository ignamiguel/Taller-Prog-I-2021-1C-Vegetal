#include <string>
#include <SDL2/SDL_image.h>
#include "PoleaVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define TIEMPO_POR_FRAME 16
#define CANT_FRAMES 3
#define SPRITE_INDEX 24

const std::string IMG_POLEAS = "res/Polea.png";

SDL_Texture *PoleaVista::texture{nullptr};
SDL_Rect PoleaVista::rectSrc;
int PoleaVista::totalPoleas = 0;

PoleaVista::PoleaVista(int x, int y, int orientation, int rotation, SDL_Renderer *renderer)
: EntidadEstaticaVista() {
    if (texture == nullptr) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_POLEAS.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_POLEAS);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        rectSrc.y = 0;
        rectSrc.w = ANCHO_POLEA;
        rectSrc.h = ALTO_POLEA;
    }

    rectDst.x = round(x * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    rectDst.y = round(y * ALTO_PANTALLA / (float)ALTO_NIVEL);
    rectDst.w = round(ANCHO_POLEA * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    rectDst.h = round(ALTO_POLEA * ALTO_PANTALLA / (float)ALTO_NIVEL);
    
    flip = (SDL_RendererFlip)(((orientation << 1) | orientation) ^ ((1 - rotation) << 1));

    ++totalPoleas;
}

void PoleaVista::mostrar() {
    tiempo = (tiempo + 1) % (TIEMPO_POR_FRAME * CANT_FRAMES);
    rectSrc.x = (tiempo / TIEMPO_POR_FRAME) * SPRITE_INDEX;

    SDL_RenderCopyEx(renderer, texture, &rectSrc, &rectDst, 0, NULL, flip);
}

PoleaVista::~PoleaVista() {
    if (!(--totalPoleas)) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}