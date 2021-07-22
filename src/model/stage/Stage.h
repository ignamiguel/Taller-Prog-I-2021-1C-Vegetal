#pragma once
#include "Tile.h"
#include "Ladder.h"
#include "Platform.h"
#include "../../utils/Constants.hpp"
#include <stack>


// Representa todos los elementos del nivel con los que Mario puede interactuar
// Contiene un arreglo de Tiles
class Stage
{
public:
    Stage();
    void addLadder(const Ladder &ladder);
    const Ladder *getLadder(float x, float y, int direction) const;
    void addPlatform(const Platform *platform);
    bool collide(float &x, float &y, float &dx, float &dy) const;
    unsigned int getPointsForCompletingLevel();

private:
    std::array<Tile, (ANCHO_NIVEL / ANCHO_TILE + 1) * (ALTO_NIVEL / ALTO_TILE + 1)> grid{};
    std::stack<unsigned char> points;
};