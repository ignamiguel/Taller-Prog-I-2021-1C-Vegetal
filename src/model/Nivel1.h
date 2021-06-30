#pragma once
#include "Nivel.h"
#include "stage/MovingPlatform.h"

class Nivel1 : public Nivel
{
public:
    Nivel1();
    void addPlayers(std::vector<Mario *> *players);
    void addEnemies(unsigned int amount);
    void update();
    estadoNivel_t *getEstado();
    ~Nivel1();

private:
    MovingPlatform *movingPlatforms[12];

    void initPlatforms();
    void updatePlatforms();
    void initLadders();
};