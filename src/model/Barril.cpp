#include "Barril.h"

const int VELOCIDAD_INICIAL_X = 1;
const int VELOCIDAD_INICIAL_Y = 0;

Barril::Barril(const float x, const float y)
: Entidad(x, y, ANCHO_BARRIL, ALTO_BARRIL), velX{VELOCIDAD_INICIAL_X}, velY{VELOCIDAD_INICIAL_Y} {}

void Barril::mover() {
    pos.x += velX;
    pos.y += velY;
}

void Barril::updateDirection(float xDirection, float yDirection) {
    velX = xDirection;
    velY = yDirection;
}


bool Barril::estaEnNivel() {
    return (pos.y < ALTO_NIVEL && pos.x > 0 && pos.x < ANCHO_NIVEL);
}