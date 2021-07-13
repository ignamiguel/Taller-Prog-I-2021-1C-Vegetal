#include "Barril.h"
#include "../utils/Constants.hpp"

Barril::Barril(float x, float y)
: Entidad(x, y, ANCHO_BARRIL, ALTO_BARRIL) {
}

void Barril::mover() {
    posY += VELOCIDAD_BARRIL;
}

void Barril::moverX(float distance) {
    posX += distance;
}

void Barril::moverY(float distance) {
    posY += distance;
}

bool Barril::estaEnNivel() {
    return (posY < ALTO_NIVEL);
}