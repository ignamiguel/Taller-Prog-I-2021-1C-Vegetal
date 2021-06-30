#pragma once
#include "NivelVista.h"
#include "PlataformaMovilVista.h"
#include "EnemigoFuegoVista.h"

class Nivel1Vista : public NivelVista
{
public:
    Nivel1Vista(SDL_Renderer *renderer, bool defaultConfig);
    void update(estadoNivel_t *estadoNivel);
    ~Nivel1Vista();

private:
    PlataformaMovilVista *plataformaVista;
    EnemigoFuegoVista *enemigoVista;
};