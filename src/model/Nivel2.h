#pragma once
#include <list>
#include "Nivel.h"
#include "Barril.h"
#include "../utils/directionalPoint.h"

class Nivel2 : public Nivel
{
public:
    Nivel2();
    void addPlayers(std::vector<Mario> &players);
    void update();
    void updateBarrelDirection(Barril* barril);
    const estadoNivel_t &getEstado();
    void checkCollisions() const;

private:
    std::list<Barril> barriles;
    size_t tick = 0;
    std::list<directionalPoint_t> directionalPoints;
    void addBarrel();
    void updateBarrels();
    void initPlatforms();
    void initLadders();
    void initHammers();
};