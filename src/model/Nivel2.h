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
        Plataforma* getPlataformaInicial();

    private:
        std::list<Barril*> barriles;
        unsigned int tick = 0;

        void addBarrel();
        void updateBarrels();
        void inicializarEscaleras();
        void inicializarPlataformas();
};