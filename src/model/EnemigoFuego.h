#pragma once
#include "Entidad.h"

class EnemigoFuego : public Entidad
{
public:
    EnemigoFuego(punto_t pos, int direccion);
    void setLimites(float min, float max);
    void mover();
    SDL_FRect dimensions();

private:
    float velX, min, max;
};