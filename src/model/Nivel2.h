#pragma once

#include <list>
#include "Nivel.h"
#include "Barril.h"
#include "nivel/Escalera.h"

class Nivel2 : public Nivel {
    public:
        Nivel2();
        void update();
        estadoNivel_t* getEstado();
        ~Nivel2();
        Escalera* getEscalera(punto_t p);

    private:
        std::vector<Barril*> barriles;
        unsigned int tick = 0;

        void addBarrel();
        void updateBarrels();
        void inicializarEscaleras();
};