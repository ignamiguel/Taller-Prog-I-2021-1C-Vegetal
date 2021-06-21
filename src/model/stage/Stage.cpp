#include <unordered_set>
#include "Stage.h"

#define ANCHO_HITBOX 8
#define STAGE_WIDTH (ANCHO_NIVEL / ANCHO_TILE)
#define STAGE_HEIGHT (ALTO_NIVEL / ALTO_TILE)
#define GRID_COLUMNS (STAGE_WIDTH + 1)
#define SLOPE 0.03125f

// Representa todos los elementos del nivel con los que Mario puede interactuar
// Contiene un arreglo de Tiles
Stage::Stage() {
    for (unsigned int i = 0; i < GRID_COLUMNS * (STAGE_HEIGHT + 1); ++i) grid[i] = new Tile();
}

void Stage::addLadder(Ladder *ladder) {
    int x = ((int)ladder->getX() + ANCHO_MARIO / 2) / STAGE_WIDTH;
    int y = (int)ladder->getBottom() / STAGE_HEIGHT;
    grid[y * GRID_COLUMNS + x]->setLadderBottom(ladder);

    y = (int)ladder->getTop() / STAGE_HEIGHT;
    grid[y * GRID_COLUMNS + x]->setLadderTop(ladder);
}

Ladder *Stage::getLadder(float x, float y, int direction) {
    unsigned int i = ((int)y / STAGE_HEIGHT) * GRID_COLUMNS + ((int)x + ANCHO_MARIO / 2) / STAGE_WIDTH;
    if (0 < direction) return grid[i]->getLadderBottom();
    if (direction < 0) return grid[i]->getLadderTop();
    return NULL;
}

void Stage::addPlatform(Platform *platform) {
    float x, max;
    platform->getLimits(&x, &max);
    unsigned int i;
    while (x < max) {
        i = ((int)platform->getY(x) / STAGE_HEIGHT) * GRID_COLUMNS + (int)x / STAGE_WIDTH;
        grid[i]->addPlatform(platform);
        x += ANCHO_TILE;
    }
    Tile *tile;
    if (grid[i] != (tile = grid[((int)platform->getY(max) / STAGE_HEIGHT) * GRID_COLUMNS + (int)max / STAGE_WIDTH])) {
        tile->addPlatform(platform);
    }
}

bool Stage::collide(float *x, float *y, float *dx, float *dy) {
    unsigned int i = ((int)(*y) / STAGE_HEIGHT) * GRID_COLUMNS + (int)(*x) / STAGE_WIDTH;
    std::unordered_set<Platform *> platforms;
    grid[i]->getPlatforms(&platforms);
    grid[i + 1]->getPlatforms(&platforms);
    grid[i + GRID_COLUMNS]->getPlatforms(&platforms);
    grid[i + (GRID_COLUMNS + 1)]->getPlatforms(&platforms);

    bool is_standing = false;
    float min, max, distanceLeft, distanceRight, distanceY;
    for (auto it = platforms.begin(); it != platforms.end(); it = platforms.erase(it)) {
        (*it)->getCurrentLimits(&min, &max);
        distanceLeft = *x - min + (ANCHO_MARIO / 2 + ANCHO_HITBOX / 2);
        distanceRight = max - *x - (ANCHO_MARIO / 2 - ANCHO_HITBOX / 2);
        if (0 < distanceLeft && 0 < distanceRight) {
            distanceY = *y + ALTO_MARIO - (*it)->getY(*x);
            if (0 <= distanceY) {
                int hit_wall = distanceY <= 3 * ALTO_MARIO / 4  && (distanceRight <= 1 || distanceLeft <= 1);
                *x -= distanceLeft * (distanceLeft <= 1 && hit_wall);
                *x += distanceRight * (distanceRight <= 1 && hit_wall);
                *dx *= !hit_wall;

                int hit_floor = !hit_wall && (distanceY <= SLOPE - *dy || (distanceY <= 1 && *dy <= 0));
                *y -= distanceY * hit_floor;
                *dy *= !hit_floor;
                *x += hit_floor * (*it)->getSpeed();
                is_standing |= hit_floor;
            }
        }
    }
    return is_standing;
}

Stage::~Stage() {
    for (Tile *tile : grid) delete tile;
}