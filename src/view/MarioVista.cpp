#include <string>
#include <SDL2/SDL_image.h>
#include "MarioVista.h"
#include "../logger.h"
#include "../utils/Constants.hpp"
#include "../utils/window.hpp"

#define MARIO_TREPANDO_INDEX 3
#define MARIO_DE_ESPALDAS_INDEX 6
#define MARIO_SALTANDO_INDEX 14
#define MARIO_MURIENDO_INDEX 16
#define MARIO_MUERTO_INDEX 18
#define MARIO_POSE_MARTILLO_INDEX 8

#define TIEMPO_FRAME_MUERTO 16
#define TIEMPO_POR_FRAME 2
#define TIEMPO_FRAME_REPOSO_MARTILLO 8
#define TIEMPO_FRAME_CORRIENDO_MARTILLO 4
#define CANT_FRAMES 4
#define CANT_FRAMES_REPOSO_MARTILLO 2
#define MARIO_SPRITE_SIZE 24

const std::string IMG_MARIO = "res/Mario.png";
const std::string IMG_HAMMER = "res/Hammer.png";

SDL_Renderer *MarioVista::renderer{nullptr};
SDL_Texture *MarioVista::texture{nullptr};

size_t MarioVista::totalJugadores = 0;

MarioVista::MarioVista(SDL_Renderer *renderer, HammerVista* vistaMartillo) {
    if (texture == nullptr) {
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
    this->vistaMartillo = vistaMartillo;
    srcRect.y = 0;
    srcRect.w = ANCHO_MARIO;
    srcRect.h = ALTO_MARIO;

    dstRect.w = round(ANCHO_MARIO * ANCHO_PANTALLA / (float)ANCHO_NIVEL);
    dstRect.h = round(ALTO_MARIO * ALTO_PANTALLA / (float)ALTO_NIVEL);

    ++totalJugadores;
}

MarioVista::MarioVista(const MarioVista &other)
: srcRect{other.srcRect}, dstRect{other.dstRect}, flip{other.flip}, tiempo{other.tiempo}, vistaMartillo{other.vistaMartillo} {
    ++totalJugadores;
}

void MarioVista::setColor(size_t color) {
    srcRect.y = color * ALTO_MARIO;
}

void MarioVista::mostrar(const estadoMario_t &estadoMario) {
    int nextX = round((int)estadoMario.pos.x * (ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    int nextY = round((int)estadoMario.pos.y * (ANCHO_PANTALLA / (float)ANCHO_NIVEL));
    switch(estadoMario.estado) {
        case REPOSO:
        case DE_ESPALDAS:
            updateReposo(estadoMario.estado, estadoMario.hasHammer);
            break;
        case CORRIENDO:
            updateCorriendo(nextX, estadoMario.hasHammer);
            break;
        case SALTANDO:
            updateSaltando(nextX);
            break;
        case TREPANDO:
            updateTrepando(nextY);
            break;
        case MURIENDO:
        case MUERTO:
            updateMuriendo(estadoMario.estado);
            break;
        case GAME_OVER:
            return;
        default:
            break;
    }
    dstRect.x = nextX;
    dstRect.y = nextY;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0., NULL, flip);
}

void MarioVista::updateReposo(char estado, bool hasHammer) {
    if(hasHammer) {
        tiempoReposo = (tiempoReposo + 1) % (TIEMPO_FRAME_REPOSO_MARTILLO * CANT_FRAMES_REPOSO_MARTILLO);
        int frameReposo = (tiempoReposo / TIEMPO_FRAME_REPOSO_MARTILLO);
        srcRect.x = (MARIO_POSE_MARTILLO_INDEX * MARIO_SPRITE_SIZE) + frameReposo * MARIO_SPRITE_SIZE;  // 0, 1, 0, 1...
        drawHammer(frameReposo);
    } else {
        srcRect.x = (estado == DE_ESPALDAS) * (MARIO_DE_ESPALDAS_INDEX * MARIO_SPRITE_SIZE);
    }
    
}

void MarioVista::updateCorriendo(int nextX, bool hasHammer) {
    int frameActual;
    if(hasHammer) {
        tiempoCorriendoConMartillo = (tiempoCorriendoConMartillo + (dstRect.x != nextX)) % (TIEMPO_FRAME_CORRIENDO_MARTILLO * CANT_FRAMES);
        flip = (SDL_RendererFlip)((dstRect.x < nextX) + (int)flip * (dstRect.x == nextX));
        frameActual = (tiempoCorriendoConMartillo / TIEMPO_FRAME_CORRIENDO_MARTILLO);
        srcRect.x = (MARIO_POSE_MARTILLO_INDEX * MARIO_SPRITE_SIZE) + frameActual * MARIO_SPRITE_SIZE;  // 0, 1, 2, 3..
        drawHammer(frameActual);
    } 
    else {
        tiempo = (tiempo + (dstRect.x != nextX)) % (TIEMPO_POR_FRAME * CANT_FRAMES);
        flip = (SDL_RendererFlip)((dstRect.x < nextX) + (int)flip * (dstRect.x == nextX));
        frameActual = (tiempo / TIEMPO_POR_FRAME);
        srcRect.x = ((frameActual & 1) << ((frameActual & 2) >> 1)) * MARIO_SPRITE_SIZE;  // 0, 1, 0, 2...
    }
        
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

void MarioVista::updateMuriendo(char estado) {
    tiempo *= srcRect.x >= MARIO_MURIENDO_INDEX * MARIO_SPRITE_SIZE;
    ++tiempo;
    if (estado == MURIENDO) {
        int sprite = (tiempo / TIEMPO_FRAME_MUERTO) % 4;
        srcRect.x = (MARIO_MURIENDO_INDEX + 1 - sprite % 2) * MARIO_SPRITE_SIZE;
        flip = (SDL_RendererFlip)((sprite > 1) * (sprite - 1));
    }
    else {
        flip = SDL_FLIP_NONE;
        srcRect.x = MARIO_MUERTO_INDEX * MARIO_SPRITE_SIZE;
    }
}

MarioVista::~MarioVista() {
    if (--totalJugadores == 0) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void MarioVista::drawHammer(int frame) {
    vistaMartillo->mostrar(dstRect.x, dstRect.y, (EstadoHammer)(frame % 2), flip);
}