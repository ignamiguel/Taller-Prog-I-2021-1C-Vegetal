#pragma once
#include "Entidad.h"
#include "mario/MarioState.h"
#include "../utils/marioStructs.h"

class MarioState;

class Mario : public Entidad
{
public:
    Mario();
    void setPos(float x, float y);
    void setControls(controls_t controls);
    void setStage(Stage *stage);
    void mover();
    void disable();
    void enable();
    estadoMario_t getEstado();
    MarioState *die();
    void reset();
    char estado;
    float velX;
    float velY;
    controls_t controls = {0, 0, 0, 0, 0};
    char contador;
    bool isGameOver();
    SDL_FRect dimensions();

private:
    MarioState *state;
    bool isEnabled;
    char lives;
};