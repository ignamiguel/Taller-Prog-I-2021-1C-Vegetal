#pragma once
#include "Entidad.h"
#include "../utils/Constants.hpp"


class Barril : public Entidad
{
public:
    Barril(float x, float y);
    void mover();
    void updateDirection(float, float);
    bool estaEnNivel();
    bool isAt(float posX, float posY);
private:
    float velX;
    float velY;
};