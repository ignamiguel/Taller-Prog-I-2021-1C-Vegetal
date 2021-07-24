#include "EnemigoFuego.h"
#include "../utils/Constants.hpp"

#define ENEMIGO_FUEGO_VEL 0.5f

#define FUEGO_X_DIF 2.f
#define FUEGO_Y_DIF 8.f

EnemigoFuego::EnemigoFuego(punto_t pos, const int direccion, const float min, const float max)
: Entidad(pos.x, pos.y, ANCHO_ENEMIGO_FUEGO, ALTO_ENEMIGO_FUEGO),
velX(direccion * ENEMIGO_FUEGO_VEL), min(min - ancho / 2), max(max - ancho / 2) {}

void EnemigoFuego::mover() {
    velX -= 2 * velX * (pos.x < min || pos.x > max);
    pos.x += velX;
}

dimensiones_t EnemigoFuego::dimensions() const {
    return {pos.x + FUEGO_X_DIF, pos.y + FUEGO_Y_DIF, pos.x + (ANCHO_ENEMIGO_FUEGO - FUEGO_X_DIF), pos.y + ALTO_ENEMIGO_FUEGO};
}