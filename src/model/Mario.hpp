#pragma once
#include "Entidad.h"
#include "mario/MarioState.h"
#include "../utils/marioStructs.h"

class Mario : public Entidad 
{
    public: 
        Mario();
        void setPos(float x, float y);
        void setControls(controls_t controls);
        void setStage(Stage *stage);
        void mover();
        estadoMario_t getEstado();

    private:
        MarioState *state;
        controls_t controls = {0, 0, 0, 0, 0};
        char estado;
        float velX;
        float velY;
};