#include "Barril.h"

const int VELOCIDAD_INICIAL_X = 1;
const int VELOCIDAD_INICIAL_Y = 0;

constexpr std::array<std::pair<punto_t,punto_t>, 12> Barril::direcciones;

Barril::Barril()
: Entidad(N2_POS_X_BARRIL, N2_POS_Y_BARRIL), vel{VELOCIDAD_INICIAL_X, VELOCIDAD_INICIAL_Y} {}

void Barril::mover() {
    pos.x += vel.x;
    pos.y += vel.y;
    if (pos == direcciones[direcciones_index].first) {
        vel = direcciones[direcciones_index++].second;
    }
}

dimensiones_t Barril::dimensions() const {
    return {pos.x, pos.y, pos.x + ANCHO_BARRIL, pos.y + ALTO_BARRIL};
}