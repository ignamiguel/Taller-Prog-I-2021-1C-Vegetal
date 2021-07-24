#include "Hammer.h"

dimensiones_t Hammer::dimensions() const {
    return {pos.x + OFFSET_X_MARTILLO, pos.y + OFFSET_Y_MARTILLO, pos.x + ANCHO_MARTILLO, pos.y + ALTO_MARTILLO};
}

void Hammer::consume() {
    pos = {0, 0};
}