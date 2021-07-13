#include <unordered_set>
#include "Stage.h"
#include <iostream>

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

void Stage::moverBarril(Barril* barril) {
    punto_t pos = barril->getPos();

    unsigned int i = ((int)(pos.y) / STAGE_HEIGHT) * GRID_COLUMNS + (int)(pos.x) / STAGE_WIDTH;
    std::unordered_set<Platform *> platforms;
    grid[i]->getPlatforms(&platforms);
    grid[i + GRID_COLUMNS]->getPlatforms(&platforms);

    float minDistance = ALTO_NIVEL;
    Platform* closestPlatform = NULL;
    float distance;

    std::cout << "" << pos.x << " ; " << pos.y << std::endl;

    // busco hacia abajo la plataforma mas cerca del barril
    for(auto it = platforms.begin(); it != platforms.end(); it = platforms.erase(it)) {
        distance = (*it)->getY(pos.x) - (pos.y + (float)ALTO_BARRIL);
        //std::cout << "plataforma: " << pos.x << ";" << (*it)->getY(pos.x) << " distancia: " << distance << std::endl;
        if (distance >= 0 && distance < minDistance) {
            //std::cout << "es la plataforma mas cerca" << distance << std::endl;
            minDistance = distance;
            closestPlatform = *it;
        }
    }

    //std::cout << "Distancia min a plataf: " << minDistance << std::endl;

    // Si no hay plataforma debajo, o esta muy lejos cae verticalmente    
    if (closestPlatform == NULL || (float)VELOCIDAD_BARRIL <= minDistance) {
        barril->mover();
        //std::cout << "no hay plataforma cerca de " << pos.x << ";" << pos.y << std::endl;
    }
    // Sino, se mueve un porcentaje de la distancia en x y otro en y 
    // dependiendo de la inclinacion de la plataforma
    else {
        float dif = VELOCIDAD_BARRIL - minDistance;

        float movX = dif * (closestPlatform->getXMovement());
        float movY = dif * (closestPlatform->getYMovement()) + minDistance;

        //std::cout << "plataforma cerca de " << pos.x << ";" << pos.y << " esta a " << dif << std::endl;
        //std::cout << "mover y: " << movY << " x: " << movX << std::endl;

        barril->moverY(movY);
        barril->moverX(movX);
    }

}

Stage::~Stage() {
    for (Tile *tile : grid) delete tile;
}