#pragma once
#include <vector>
#include "stage/Stage.h"
#include "Mario.hpp"
#include "EnemigoFuego.h"
#include "Hammer.h"
#include "../utils/estadoJuego.h"

// Representa el escenario del juego
// contiene a Stage (plataformas y escaleras)
class Nivel
{
public:
    void addPlayers(std::vector<Mario> &players);
    virtual void update() = 0;
    virtual const estadoNivel_t &getEstado() = 0;
    bool isComplete() const;
    bool collision(dimensiones_t, dimensiones_t) const;
    bool getIsGameOver();
    virtual ~Nivel() = default;

protected:
    Stage stage;
    estadoNivel_t estadoNivel;
    std::vector<EnemigoFuego> enemies;
    std::vector<Hammer> hammers;
    std::vector<Platform> platforms;
    std::vector<Mario> *players;
};