#pragma once
#include "Nivel.h"
#include "stage/MovingPlatform.h"

class Nivel1 : public Nivel
{
public:
    Nivel1();
    void addPlayers(std::vector<Mario> &players);
    void addEnemies(unsigned int amount);
    void update();
    const estadoNivel_t &getEstado();
    void checkCollisions() const;

private:
    std::array<MovingPlatform, 12> movingPlatforms {{{0, 0}, {1, 0}, {2, 0},
                                                     {0, 1}, {1, 1}, {2, 1},
                                                     {0, 2}, {1, 2}, {2, 2},
                                                     {0, 3}, {1, 3}, {2, 3}}};

    void initPlatforms();
    void initLadders();
    void initHammers();
};