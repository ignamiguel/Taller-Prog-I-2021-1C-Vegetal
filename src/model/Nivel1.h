#pragma once

#include <list>
#include "PlataformaMovil.h"
#include "Nivel.h"
#include "nivel/Escalera.h"
#include "../utils/Constants.hpp"

class Nivel1 : public Nivel
{
    public:
        Nivel1();
        void update();
        estadoNivel_t* getEstado();
        ~Nivel1();
        Plataforma* getPlataformaInicial();
        Plataforma* getPlataforma(punto_t p);

    private:
        std::list<PlataformaMovil*> plataformasMoviles;

        void initPlataformas();
        void updatePlatforms();
        void inicializarEscaleras();
};