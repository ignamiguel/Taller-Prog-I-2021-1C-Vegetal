#pragma once
#include "../utils/estadoJuego.h"

/* Interfaz que representa cualquier escena que se puede mostrar al cliente */
class SceneVista
{
public:
    virtual void update(const estadoJuego_t &estadoJuego) = 0;
    virtual ~SceneVista() = default;
};