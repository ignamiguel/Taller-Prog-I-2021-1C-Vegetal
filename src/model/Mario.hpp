#pragma once

#include "Entidad.h"
#include "../utils/estadoMario.h"
#include "mario/MarioState.h"
#include "Nivel.h"
#include "Plataforma.h"

#define SPACE 128
#define UP 8
#define DOWN 4
#define LEFT 2
#define RIGHT 1

class MarioState;
class Nivel;

class Mario : public Entidad 
{
    public: 
        Mario();
        void setPos(float x, float y);
        void mover();
        void setEstado(char controls);
        estadoMario_t getEstado();
        void setNivel(Nivel* nivel);
        Nivel* getNivel();
        Plataforma* getPlataformaActual();
        void setPlataformaActual(Plataforma* p);

    private:
        MarioState* state;
        Nivel* nivel;
        Plataforma* plataformaActual;
};