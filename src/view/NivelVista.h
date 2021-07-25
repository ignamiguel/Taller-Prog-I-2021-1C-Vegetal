#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "SceneVista.h"
#include "MarioVista.h"
#include "EntidadEstaticaVista.h"
#include "HammerVista.h"
#include "StatsVista.h"

class NivelVista : public SceneVista
{
public:
    NivelVista(SDL_Renderer *renderer, const char *clientUsername);
    ~NivelVista();

protected:
    void setBackground(const std::string &rutaImagen);

    SDL_Renderer *renderer;
    SDL_Texture *texture;
    StatsVista statsVista;
    HammerVista hammerVista;
    std::vector<MarioVista> jugadoresVista;
    std::vector<EntidadEstaticaVista *> entidadesVista;
    char clientUsername[4];
private:
    void addPlayers(size_t n);
};