#pragma once
#include "../utils/punto.h"
#include "../utils/dimensiones.h"

class Entidad
{
public:
    Entidad(float posX, float posY, int ancho, int alto);
    virtual void mover() = 0; //separar entidades en entidades estaticas y dinamicas
    virtual ~Entidad() = default;
    punto_t pos;
    virtual dimensiones_t dimensions() const = 0;
    bool isEnabled = true;
protected:
    int ancho;
    int alto;
    
};