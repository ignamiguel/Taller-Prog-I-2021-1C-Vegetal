#pragma once
#include <list>
#include "Nivel.h"
#include "Barril.h"

class Nivel2 : public Nivel
{
public:
    Nivel2();
    void update();
    const estadoNivel_t &getEstado();

private:
    std::list<Barril> barriles;
    size_t tick = 0;

    void initPlatforms();
    void initLadders();
    void initHammers();

    void updateBarrels();
    void checkCollisions();
    void deleteDisabledBarrels();
};
