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

directionalPoint_t Barril::getDirectionalPos () {
    return (directionalPoint_t){pos.x, pos.y, velX, velY};
}


bool Barril::estaEnNivel() {
    return (pos.x > 0 && pos.x < ANCHO_NIVEL);
}

dimensiones_t Barril::dimensions() const {
    return {pos.x, pos.y, pos.x + ANCHO_BARRIL, pos.y + ALTO_BARRIL};
 }