#pragma once
#include "../utils/estadoJuego.h"

/* Representa a todas las clases que tienen un estadoJuego */
class Scene
{
public:
    virtual void update() = 0;
    virtual const estadoNivel_t &getEstado() = 0;
    virtual bool isComplete() const = 0;
    virtual ~Scene() = default;

protected:
    estadoNivel_t estadoNivel;
};