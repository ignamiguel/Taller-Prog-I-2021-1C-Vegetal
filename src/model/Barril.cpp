#include "Barril.h"
#include <iostream>

#define VELOCIDAD_BARRIL 1

Barril::Barril(const float x, const float y)
: Entidad(x, y, ANCHO_BARRIL, ALTO_BARRIL), velX{0}, velY{VELOCIDAD_BARRIL} {
    velX = VELOCIDAD_BARRIL;
    velY = 0;
}

void Barril::mover() {
    std::cout << "mueve " << pos.x << " + " << velX << " = " << pos.x + velX << std::endl;
    pos.x += velX;
    pos.y += velY;
}

void Barril::updateDirection(float xDirection, float yDirection) {
    velX = xDirection;
    velY = yDirection;
    std::cout << "barril en " << pos.x << " ; " << pos.y << std::endl;
    std::cout << "velocidad " << velX << " ; " << velY << std::endl;
}


bool Barril::estaEnNivel() {
    return (pos.y < ALTO_NIVEL && pos.x > 0 && pos.x < ANCHO_NIVEL);
}