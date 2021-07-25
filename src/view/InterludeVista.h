#pragma once
#include <SDL2/SDL.h>
#include "SceneVista.h"

class InterludeVista : public SceneVista
{
public:
    InterludeVista(SDL_Renderer *renderer);
    void update(const estadoJuego_t &estadoJuego) override;
private:
    SDL_Renderer *renderer;
    size_t maxPlayers;
};