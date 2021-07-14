#include <string>
#include <SDL2/SDL_image.h>
#include "EnemigoFuegoVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define TIEMPO_POR_FRAME 8
#define CANT_FRAMES 2
#define SPRITE_INDEX_SIZE 24

const std::string IMG_ENEMIGO_FUEGO = "res/Enemy1.png";

SDL_Renderer *EnemigoFuegoVista::renderer{nullptr};
SDL_Texture *EnemigoFuegoVista::texture{nullptr};
SDL_Rect EnemigoFuegoVista::srcRect, EnemigoFuegoVista::dstRect;

EnemigoFuegoVista::EnemigoFuegoVista(SDL_Renderer *renderer) {
    if (this->texture == nullptr) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_ENEMIGO_FUEGO.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_ENEMIGO_FUEGO);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
        this->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    srcRect = {0, 0, ANCHO_ENEMIGO_FUEGO, ALTO_ENEMIGO_FUEGO};

    dstRect.w = round(ANCHO_ENEMIGO_FUEGO * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(ALTO_ENEMIGO_FUEGO * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void EnemigoFuegoVista::startRender() {
    updated = 0;
}

void EnemigoFuegoVista::mover(punto_t pos) {
    dstRect.x = round(pos.x * (ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    dstRect.y = round(pos.y * (ALTO_PANTALLA / (float)ALTO_NIVEL));
}

void EnemigoFuegoVista::mostrar() {
    tiempo = (tiempo + (updated == 0)) % (TIEMPO_POR_FRAME * CANT_FRAMES);
    srcRect.x = (tiempo / TIEMPO_POR_FRAME) * SPRITE_INDEX_SIZE;

    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
    updated = 1;
}

EnemigoFuegoVista::~EnemigoFuegoVista() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}