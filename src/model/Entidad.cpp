#include "Entidad.h"

Entidad::Entidad(float posX, float posY, int ancho, int alto) {
    this->posX = posX;
    this->posY = posY;
    this->ancho = ancho;
    this->alto = alto;
}

punto_t Entidad::getPos() {
    punto_t pos;
    pos.x = posX;
    pos.y = posY;
    
    return pos;
}

SDL_FRect Entidad::dimensions() {
    return {posX, posY, posX + ancho, posY + alto};
 }