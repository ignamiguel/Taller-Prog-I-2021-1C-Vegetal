#pragma once
#include "../utils/punto.h"
#include "../utils/dimensiones.h"

const int ANCHO_MARTILLO = 13;
const int ALTO_MARTILLO = 16;
const int OFFSET_X_MARTILLO = 4;
const int OFFSET_Y_MARTILLO = 6;

class Hammer
{
public:
    Hammer(punto_t pos) : pos(pos) {}
    dimensiones_t dimensions() const;
    void consume();
    punto_t pos;
private:
};