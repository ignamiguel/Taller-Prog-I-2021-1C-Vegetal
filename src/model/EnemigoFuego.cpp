#include "EnemigoFuego.h"
#include "../utils/Constants.hpp"

#define ENEMIGO_FUEGO_VEL 0.5f

const int FUEGO_ALTO_REAL = 6;
const int FUEGO_ANCHO_REAL = 12;

EnemigoFuego::EnemigoFuego(punto_t pos, int direccion)
: Entidad(pos.x, pos.y, ANCHO_ENEMIGO_FUEGO, ALTO_ENEMIGO_FUEGO, FUEGO_ANCHO_REAL, FUEGO_ALTO_REAL) {
    this->velX = direccion * ENEMIGO_FUEGO_VEL;
}

void EnemigoFuego::setLimites(float min, float max) {
    this->min = min - ancho / 2;
    this->max = max - ancho / 2;
}

void EnemigoFuego::mover() {
    velX -= 2 * velX * (posX < min || posX > max);
    posX += velX;
}