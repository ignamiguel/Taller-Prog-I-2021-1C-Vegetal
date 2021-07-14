#include <string>
#include <SDL2/SDL_image.h>
#include "FuegoVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define TIEMPO_POR_FRAME 4
#define CANT_FRAMES 4
#define SPRITE_INDEX_SIZE 24

const std::string IMG_FUEGO = "res/Fuego.png";

SDL_Texture *FuegoVista::texture{nullptr};
SDL_Rect FuegoVista::srcRect;
int FuegoVista::totalFuegos = 0;

FuegoVista::FuegoVista(int x, int y, SDL_Renderer* renderer)
: EntidadEstaticaVista() {
    if (texture == nullptr) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_FUEGO.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_FUEGO);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        srcRect.y = 0;
        srcRect.w = ANCHO_FUEGO;
        srcRect.h = ALTO_FUEGO;
    }

    dstRect.x = round(x * (ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    dstRect.y = round(y * (ALTO_PANTALLA / (float)ALTO_NIVEL));
    dstRect.w = round(ANCHO_FUEGO * ANCHO_PANTALLA / ANCHO_NIVEL);
    dstRect.h = round(ALTO_FUEGO * ALTO_PANTALLA / ALTO_NIVEL);

    ++totalFuegos;
}

void FuegoVista::mostrar() {
    tiempo = (tiempo + 1) % (TIEMPO_POR_FRAME * CANT_FRAMES);
    srcRect.x = (tiempo / TIEMPO_POR_FRAME) * SPRITE_INDEX_SIZE;

    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

FuegoVista::~FuegoVista() {
    if (--totalFuegos == 0) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}