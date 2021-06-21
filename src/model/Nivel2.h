#pragma once
#include "Nivel.h"
#include "Barril.h"

class Nivel2 : public Nivel
{
    public:
        Nivel2();
        void addPlayers(std::vector<Mario *> *players);
        void update();
        estadoNivel_t* getEstado();
        ~Nivel2();

    private:
        std::list<Barril*> barriles;
        unsigned int tick = 0;

        void addBarrel();
        void updateBarrels();
        void initPlatforms();
        void initLadders();
};