#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "../utils/estadoJuego.h"
#include "MarioVista.h"
#include "EntidadEstaticaVista.h"
#include "StatsVista.h"

class NivelVista
{
public:
    NivelVista(SDL_Renderer *renderer, const char *clientUsername);
    void setBackground(const std::string &rutaImagen);
    virtual void update(const estadoJuego_t &estadoJuego) = 0;
    virtual ~NivelVista();

protected:
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    StatsVista statsVista;
    std::vector<MarioVista> jugadoresVista;
    std::vector<EntidadEstaticaVista *> entidadesVista;
    char clientUsername[4];
private:
    void addPlayers(size_t n);
};