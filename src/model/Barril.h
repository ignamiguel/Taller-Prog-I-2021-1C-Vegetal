#pragma once
#include "Entidad.h"
#include "../utils/Constants.hpp"
#include "../utils/directionalPoint.h"


class Barril : public Entidad
{
public:
    Barril(float x, float y);
    void mover();
    void updateDirection(float, float);
    bool estaEnNivel();
    directionalPoint_t getDirectionalPos ();
    dimensiones_t dimensions() const;

private:
    float velX;
    float velY;
};