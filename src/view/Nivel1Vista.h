#pragma once
#include "NivelVista.h"
#include "PlataformaMovilVista.h"
#include "EnemigoFuegoVista.h"

class Nivel1Vista : public NivelVista
{
public:
    Nivel1Vista(SDL_Renderer *renderer, const char* clientUsername);
    void update(const estadoJuego_t &estadoJuego) override;

private:
    PlataformaMovilVista plataformaVista;
    EnemigoFuegoVista enemigoVista;
};