#pragma once
#include "MarioState.h"

class AireState : public MarioState
{
public:
    MarioState *update(float *x, float *y, float *xSpeed, float *ySpeed, char *estado, controls_t);
    static AireState *getInstance();

private:
    AireState();
    static AireState *instance;
};