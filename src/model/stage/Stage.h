#pragma once
#include "Tile.h"
#include "Ladder.h"
#include "Platform.h"
#include "../../utils/Constants.hpp"

// Representa todos los elementos del nivel con los que Mario puede colsionar
// Contiene un arreglo de Tiles
class Stage
{
public:
    Stage();
    void addLadder(Ladder *ladder);
    Ladder *getLadder(float x, float y, int drection);
    void addPlatform(Platform *platform);
    bool collide(float *x, float *y, float *dx, float *dy);
    ~Stage();

private:
    Tile *grid[(ANCHO_NIVEL / ANCHO_TILE + 1) * (ALTO_NIVEL / ALTO_TILE + 1)];
};