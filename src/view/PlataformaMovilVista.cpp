#include <string>
#include <SDL2/SDL_image.h>
#include "PlataformaMovilVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

const std::string IMG_PLATAFORMA_MOVIL = "res/PlataformaMovil.png";

SDL_Renderer *PlataformaMovilVista::renderer{nullptr};
SDL_Texture *PlataformaMovilVista::texture{nullptr};
int PlataformaMovilVista::totalPlataformas = 0;

PlataformaMovilVista::PlataformaMovilVista(SDL_Renderer *renderer) {
    if (this->texture == nullptr) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_PLATAFORMA_MOVIL.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_PLATAFORMA_MOVIL);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
        this->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    dstRect.w = round(ANCHO_PLATAFORMA * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(ALTO_PLATAFORMA * ALTO_PANTALLA / (float)ALTO_NIVEL);

    ++totalPlataformas;
}

void PlataformaMovilVista::mover(punto_t pos) {
    dstRect.x = round(pos.x * (ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    dstRect.y = round(pos.y * (ALTO_PANTALLA / (float)ALTO_NIVEL));
}

void PlataformaMovilVista::mostrar() {
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}

PlataformaMovilVista::~PlataformaMovilVista() {
    if (--totalPlataformas == 0) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}