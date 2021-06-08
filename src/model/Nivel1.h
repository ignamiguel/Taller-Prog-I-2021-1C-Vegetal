#pragma once

#include <list>
#include "PlataformaMovil.h"
#include "Nivel.h"
#include "nivel/Escalera.h"

class Nivel1 : public Nivel
{
    public:
        Nivel1();
        void update();
        estadoNivel_t* getEstado();
        ~Nivel1();
        Escalera* getEscalera(punto_t p);

    private:
        std::list<PlataformaMovil*> plataformasMoviles;

        void initPlataformas();
        void updatePlatforms();
        void inicializarEscaleras();
};