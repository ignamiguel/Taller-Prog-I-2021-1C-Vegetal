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
    void checkCollisions ();
    ~Nivel1();

private:
    MovingPlatform *movingPlatforms[12];

    void initPlatforms();
    void updatePlatforms();
    void initLadders();
};