#include <string>
#include <SDL2/SDL_image.h>
#include "PaulineVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

const std::string IMG_PAULINE = "res/Pauline.png";
const std::string IMG_HELP = "res/PaulineHelp.png";

SDL_Texture *PaulineVista::texturePauline = NULL;
SDL_Texture *PaulineVista::textureHelp = NULL;

PaulineVista::PaulineVista(SDL_Renderer* renderer)
: EntidadEstaticaVista() {
    this->renderer = renderer;
    if (texturePauline == NULL) {
        SDL_Surface* surface = IMG_Load(IMG_PAULINE.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_PAULINE);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
        texturePauline = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        surface = IMG_Load(IMG_HELP.c_str());
        if (surface == NULL) {
            logger::Logger::getInstance().logError("Image not found: " + IMG_HELP);
            logger::Logger::getInstance().logDebug("Loading default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        }
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 0));
        textureHelp = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    srcRect = {0, 0, PAULINE_ANCHO, PAULINE_ALTO};
    dstRect.x = round(PAULINE_POS_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.y = round(PAULINE_POS_Y * ALTO_PANTALLA / (float)ALTO_NIVEL);
    dstRect.w = round(PAULINE_ANCHO * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(PAULINE_ALTO * ALTO_PANTALLA / (float)ALTO_NIVEL);

    helpSrcRect = {0, 0, HELP_ANCHO, HELP_ALTO};
    helpDstRect.x = round(HELP_POS_X * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    helpDstRect.y = round(HELP_POS_Y * ALTO_PANTALLA / (float)ALTO_NIVEL);
    helpDstRect.w = round(HELP_ANCHO * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    helpDstRect.h = round(HELP_ALTO * ALTO_PANTALLA / (float)ALTO_NIVEL);
}

void PaulineVista::mostrar() {
    ++counter;
    int animate = 32 <= counter && counter < 96;
    srcRect.x = (fin * 2 + (1 - fin) * animate * (1 - (counter / 8) % 2)) * 24;
    SDL_RenderCopy(renderer, texturePauline, &srcRect, &dstRect);

    if (animate && !fin) SDL_RenderCopy(renderer, textureHelp, &helpSrcRect, &helpDstRect);
}

PaulineVista::~PaulineVista() {
    SDL_DestroyTexture(texturePauline);
    SDL_DestroyTexture(textureHelp);
    texturePauline = NULL;
}