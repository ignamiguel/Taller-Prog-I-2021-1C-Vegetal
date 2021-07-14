#include "Barril.h"

#define VELOCIDAD_BARRIL 1

const int BARRIL_ANCHO_REAL = 12;

Barril::Barril(const float x, const float y)
: Entidad(x, y, ANCHO_BARRIL, ALTO_BARRIL), velX{0}, velY{VELOCIDAD_BARRIL} {}

void Barril::mover() {
    pos.y += this->velY;
}

dimensiones_t Barril::dimensions() {
    return {pos.x, pos.y, pos.x + BARRIL_ANCHO_REAL, pos.y + ALTO_BARRIL};
 }