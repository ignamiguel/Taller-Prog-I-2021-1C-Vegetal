#pragma once
#include <list>
#include <vector>
#include <SDL2/SDL.h>
#include "stage/Stage.h"
#include "Mario.hpp"
#include "EnemigoFuego.h"
#include "../utils/estadoJuego.h"

// Representa el escenario del juego
// contiene a Stage (elementos colisionables)
class Nivel
{
public:
    Nivel();
    virtual void addPlayers(std::vector<Mario *> *players) = 0;
    virtual void update() = 0;
    virtual estadoNivel_t *getEstado() = 0;
    bool isComplete();
    virtual ~Nivel();
    bool collision(SDL_FRect, SDL_FRect);
    void checkCollisions ();

protected:
    Stage *stage;
    std::vector<Platform *> platforms;
    std::list<EnemigoFuego *> enemies;
    std::vector<Mario *> *players;
    estadoNivel_t *estadoNivel;
};