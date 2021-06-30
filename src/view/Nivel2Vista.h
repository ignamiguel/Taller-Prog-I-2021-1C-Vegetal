#pragma once
#include "NivelVista.h"
#include "BarrilVista.h"

class Nivel2Vista : public NivelVista
{
public:
    Nivel2Vista(SDL_Renderer *renderer, bool defaultConfig);
    void update(estadoNivel_t *estadoNivel);
    ~Nivel2Vista();

private:
    BarrilVista *barrilVista;
};