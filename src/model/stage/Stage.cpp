#include <unordered_set>
#include "Stage.h"

#define ANCHO_HITBOX 8
#define STAGE_WIDTH (ANCHO_NIVEL / ANCHO_TILE)
#define STAGE_HEIGHT (ALTO_NIVEL / ALTO_TILE)
#define GRID_COLUMNS (STAGE_WIDTH + 1)
#define SLOPE 0.03125f

void Stage::addLadder(const Ladder &ladder_) {
    const int x = ((int)ladder_.x + ANCHO_MARIO / 2) / STAGE_WIDTH;
    int y = (int)ladder_.bottom / STAGE_HEIGHT;
    Ladder *ladder = new Ladder(ladder_);
    grid[y * GRID_COLUMNS + x].setLadderBottom(ladder);

    y = (int)ladder_.top / STAGE_HEIGHT;
    grid[y * GRID_COLUMNS + x].setLadderTop(ladder);
}

const Ladder *Stage::getLadder(const float x, const float y, const int direction) const {
    const size_t i = ((int)y / STAGE_HEIGHT) * GRID_COLUMNS + ((int)x + ANCHO_MARIO / 2) / STAGE_WIDTH;
    if (0 < direction) return grid[i].getLadderBottom();
    if (direction < 0) return grid[i].getLadderTop();
    return nullptr;
}

void Stage::addPlatform(const Platform *platform) {
    float x, max;
    platform->getLimits(x, max);
    size_t i;
    while (x < max) {
        i = ((size_t)platform->getY(x) / STAGE_HEIGHT) * GRID_COLUMNS + (size_t)x / STAGE_WIDTH;
        grid[i].addPlatform(platform);
        x += ANCHO_TILE;
    }
    Tile &tile = grid[((size_t)platform->getY(max) / STAGE_HEIGHT) * GRID_COLUMNS + (size_t)max / STAGE_WIDTH];
    if (&grid[i] != &tile) {
        tile.addPlatform(platform);
    }
}

bool Stage::collide(float &x, float &y, float &dx, float &dy) const {
    const size_t i = ((int)y / STAGE_HEIGHT) * GRID_COLUMNS + (int)x / STAGE_WIDTH;
    std::unordered_set<const Platform *> platforms;
    grid[i].getPlatforms(platforms);
    grid[i + 1].getPlatforms(platforms);
    grid[i + GRID_COLUMNS].getPlatforms(platforms);
    grid[i + (GRID_COLUMNS + 1)].getPlatforms(platforms);

    bool is_standing = false;
    float min, max;
    for (auto it = platforms.begin(); it != platforms.end(); ++it) {
        (*it)->getCurrentLimits(min, max);
        const float distanceLeft = x - min + (ANCHO_MARIO / 2 + ANCHO_HITBOX / 2);
        const float distanceRight = max - x - (ANCHO_MARIO / 2 - ANCHO_HITBOX / 2);
        if (0 < distanceLeft && 0 < distanceRight) {
            const float distanceY = y + ALTO_MARIO - (*it)->getY(x);
            if (0 <= distanceY) {
                const int hit_wall = distanceY <= 3 * ALTO_MARIO / 4  && (distanceRight <= 1 || distanceLeft <= 1);
                x -= distanceLeft * (distanceLeft <= 1 && hit_wall);
                x += distanceRight * (distanceRight <= 1 && hit_wall);
                dx *= !hit_wall;

                const int hit_floor = !hit_wall && (distanceY <= SLOPE - dy || (distanceY <= 1 && dy <= 0));
                y -= distanceY * hit_floor;
                dy *= !hit_floor;
                x += hit_floor * (*it)->getSpeed();
                is_standing |= hit_floor;
            }
        }
    }
    return is_standing;
}