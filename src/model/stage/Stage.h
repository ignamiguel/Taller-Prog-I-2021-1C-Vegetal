#pragma once
#include "Tile.h"
#include "Ladder.h"
#include "Platform.h"
#include "../../utils/Constants.hpp"

// Representa todos los elementos del nivel con los que Mario puede interactuar
// Contiene un arreglo de Tiles
class Stage
{
public:
    void addLadder(const Ladder &ladder);
    const Ladder *getLadder(float x, float y, int drection) const;
    void addPlatform(const Platform *platform);
    bool collide(float &x, float &y, float &dx, float &dy) const;

private:
    std::array<Tile, (ANCHO_NIVEL / ANCHO_TILE + 1) * (ALTO_NIVEL / ALTO_TILE + 1)> grid{};
};