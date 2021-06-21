#pragma once
#include "MarioState.h"

class SueloState : public MarioState
{
    public:
        MarioState *update(float *x, float *y, float *xSpeed, float *ySpeed, char *estado, controls_t controls);
        static SueloState *getInstance();
    
    private:
        SueloState();
        static SueloState *instance;
};