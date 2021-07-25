#pragma once
#include "NivelVista.h"
#include "BarrilVista.h"

class Nivel2Vista : public NivelVista
{
public:
    Nivel2Vista(SDL_Renderer *renderer, const char* clientUsername);
    void update(const estadoJuego_t &estadoJuego) override;

private:
    BarrilVista barrilVista;
};