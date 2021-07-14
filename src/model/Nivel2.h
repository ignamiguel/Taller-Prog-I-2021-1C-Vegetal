#pragma once
#include <list>
#include "Nivel.h"
#include "Barril.h"

class Nivel2 : public Nivel
{
public:
    Nivel2();
    void addPlayers(std::vector<Mario> &players);
    void update();
    const estadoNivel_t &getEstado();
    void checkCollisions ();
    ~Nivel2();

private:
    std::list<Barril> barriles;
    size_t tick = 0;

    void addBarrel();
    void updateBarrels();
    void initPlatforms();
    void initLadders();
};