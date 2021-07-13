#pragma once
#include "Entidad.h"

class Barril : public Entidad
{
public:
    Barril(float x, float y);
    void mover ();
    void moverX(float);
    void moverY(float);
    bool estaEnNivel();
};