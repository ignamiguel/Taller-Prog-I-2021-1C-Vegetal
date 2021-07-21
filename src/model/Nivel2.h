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
    void getEstado(estadoNivel_t &estadoNivel) const override;
    void checkCollisions() const;

private:
    std::list<Barril> barriles;
    size_t tick = 0;

    void initPlatforms();
    void initLadders();
    void initHammers();

    void addBarrel();
    void updateBarrels();
};