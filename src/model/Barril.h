#pragma once
#include "Entidad.h"

class Barril : public Entidad
{
public:
    Barril(float x, float y);
    void mover();
    bool estaEnNivel();

private:
    float velX;
    float velY;
};