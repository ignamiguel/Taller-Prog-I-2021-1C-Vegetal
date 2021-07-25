#pragma once
#include <vector>
#include "Scene.h"
#include "stage/Stage.h"
#include "Mario.hpp"
#include "EnemigoFuego.h"
#include "Hammer.h"

/* Representa un nivel del juego, contiene a Stage (plataformas y escaleras) */
class Nivel : public Scene
{
public:
    Nivel(std::vector<Mario> *players);
    bool isComplete() const override;

protected:
    bool collision(dimensiones_t, dimensiones_t) const;

    Stage stage;
    std::vector<EnemigoFuego> enemies;
    std::vector<Hammer> hammers;
    std::vector<Platform> platforms;
    std::vector<Mario> *players;
};