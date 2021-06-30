#include <string>
#include <SDL2/SDL_image.h>
#include "MarioVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define MARIO_SPRITE_SIZE 24
#define MARIO_SALTANDO_INDEX 14
#define MARIO_TREPANDO_INDEX 3
#define MARIO_DE_ESPALDAS_INDEX 6
#define TIEMPO_POR_FRAME 2
#define CANT_FRAMES 4

const std::string IMG_MARIO = "res/Mario.png";

SDL_Renderer *MarioVista::renderer = NULL;
SDL_Texture *MarioVista::texture = NULL;

int MarioVista::totalJugadores = 0;

MarioVista::MarioVista(SDL_Renderer *renderer) {
    if (texture == NULL) {
        this->renderer = renderer;
        SDL_Surface* surface = IMG_Load(IMG_MARIO.c_str());

        if(surface == NULL) {
            logger::Logger::getInstance().logError("Mario image not found: " + IMG_MARIO);
            logger::Logger::getInstance().logDebug("Loading Mario default image: " + IMG_DEFAULT);
            surface = IMG_Load(IMG_DEFAULT.c_str());
        } else SDL_SetColorKey(surface, SDL_TRUE, *(Uint32*)(surface->pixels));
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    srcRect.y = 0;
    srcRect.w = ANCHO_MARIO;
    srcRect.h = ALTO_MARIO;

    dstRect.w = round(ANCHO_MARIO * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(ALTO_MARIO * ALTO_PANTALLA / (float)ALTO_NIVEL);

    tiempo = 0;
    flip = SDL_FLIP_HORIZONTAL;

    ++totalJugadores;
}

void MarioVista::setColor(int nroJugador) {
    srcRect.y = nroJugador * ALTO_MARIO;
}

void MarioVista::mostrar(punto_t pos, char estado) {
    int nextX = round((int)pos.x * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    int nextY = round((int)pos.y * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    switch(estado) {
        case REPOSO:
        case DE_ESPALDAS:
            updateReposo(estado);
            break;
        case CORRIENDO:
            updateCorriendo(nextX);
            break;
        case SALTANDO:
            updateSaltando(nextX);
            break;
        case TREPANDO:
            updateTrepando(nextY);
            break;
        case DESCONECTADO:
            updateDesconectado(nextX, nextY);
            return;
        default:
            break;
    }
    dstRect.x = nextX;
    dstRect.y = nextY;

    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0., NULL, flip);
}

void MarioVista::updateReposo(char estado) {
    srcRect.x = (estado == DE_ESPALDAS) * (MARIO_DE_ESPALDAS_INDEX * MARIO_SPRITE_SIZE);
}

void MarioVista::updateCorriendo(int nextX) {
    tiempo = (tiempo + (dstRect.x != nextX)) % (TIEMPO_POR_FRAME * CANT_FRAMES);
    flip = (SDL_RendererFlip)((dstRect.x < nextX) + (int)flip * (dstRect.x == nextX));
    int frameActual = (tiempo / TIEMPO_POR_FRAME);
    srcRect.x = ((frameActual & 1) << ((frameActual & 2) >> 1)) * MARIO_SPRITE_SIZE;  // 0, 1, 0, 2...
}

void MarioVista::updateSaltando(int nextX) {
    flip = (SDL_RendererFlip)((dstRect.x < nextX) + (int)flip * (dstRect.x == nextX));
    srcRect.x = MARIO_SALTANDO_INDEX * MARIO_SPRITE_SIZE;
}

void MarioVista::updateTrepando(int nextY) {
    tiempo += dstRect.y != nextY;
    flip = (SDL_RendererFlip)((tiempo / 8) % 2);
    srcRect.x = MARIO_TREPANDO_INDEX * MARIO_SPRITE_SIZE;
}

void MarioVista::updateDesconectado(int nextX, int nextY) {
    SDL_Rect _srcRect;
    flip = SDL_FLIP_HORIZONTAL;
    _srcRect.w = ANCHO_MARIO;
    _srcRect.h = ALTO_MARIO;
    _srcRect.x = 0;
    _srcRect.y = 64;

    dstRect.x = nextX;
    dstRect.y = nextY;

    SDL_RenderCopyEx(renderer, texture, &_srcRect, &dstRect, 0., NULL, flip);
}

MarioVista::~MarioVista() {
    if (--totalJugadores == 0) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}