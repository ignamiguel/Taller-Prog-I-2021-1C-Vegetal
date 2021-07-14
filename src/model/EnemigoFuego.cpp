#include "EnemigoFuego.h"
#include "../utils/Constants.hpp"

#define ENEMIGO_FUEGO_VEL 0.5f

const int FUEGO_ALTO_REAL = 6;
const int FUEGO_ANCHO_REAL = 12;

EnemigoFuego::EnemigoFuego(punto_t pos, const int direccion, const float min, const float max)
: Entidad(pos.x, pos.y, ANCHO_ENEMIGO_FUEGO, ALTO_ENEMIGO_FUEGO),
velX(direccion * ENEMIGO_FUEGO_VEL), min(min - ancho / 2), max(max - ancho / 2) {}

void EnemigoFuego::mover() {
    velX -= 2 * velX * (pos.x < min || pos.x > max);
    pos.x += velX;
}

dimensiones_t EnemigoFuego::dimensions() {
    return {pos.x, pos.x, pos.x + FUEGO_ANCHO_REAL, pos.y + FUEGO_ALTO_REAL};
 }
