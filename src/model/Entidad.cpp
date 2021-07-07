#include "Entidad.h"

Entidad::Entidad(float posX, float posY, int ancho, int alto, int ancho_real , int alto_real) {
    this->posX = posX;
    this->posY = posY;
    this->ancho = ancho;
    this->alto = alto;
    this->ancho_real = ancho_real;
    this->alto_real = alto_real;
}

punto_t Entidad::getPos() {
    punto_t pos;
    pos.x = posX;
    pos.y = posY;
    
    return pos;
}

SDL_FRect Entidad::dimensions() {
    return {posX, posY, posX + ancho_real, posY - alto_real};
 }