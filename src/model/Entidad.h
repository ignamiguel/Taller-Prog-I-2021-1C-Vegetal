#pragma once
#include "../utils/punto.h"
#include "../utils/dimensiones.h"

class Entidad
{
public:
    Entidad(float posX, float posY) : pos{posX, posY} {};
    virtual void mover() = 0;
    virtual ~Entidad() = default;
    virtual dimensiones_t dimensions() const = 0;

    punto_t pos;
    bool isEnabled = true;   
};