#include "Barril.h"
#include "../utils/Constants.hpp"

#define VELOCIDAD_BARRIL 1

const int BARRIL_ANCHO_REAL = 12;

Barril::Barril(float x, float y)
: Entidad(x, y, ANCHO_BARRIL, ALTO_BARRIL) {
    this->velX = 0;
    this->velY = VELOCIDAD_BARRIL;
}

void Barril::mover() {
    posY += this->velY;
}

bool Barril::estaEnNivel() {
    return (posY < ALTO_NIVEL);
}

SDL_FRect Barril::dimensions() {
    return {posX, posY, posX + BARRIL_ANCHO_REAL, posY - ALTO_BARRIL};
 }