#pragma once
#include "../stage/Stage.h"
#include "../../utils/marioStructs.h"

#define MARIO_VEL_X 0.5f
#define MARIO_VEL_TREPAR 0.5f
#define MARIO_VEL_SALTO 2.f
#define GRAVEDAD (- (MARIO_VEL_SALTO / 16))

class MarioState {
    public:
        void setStage(Stage *stage);
        virtual MarioState* update(float *x, float *y, float *xSpeed, float *ySpeed, char *estado, controls_t controls) = 0;
        virtual ~MarioState() = default;

    protected:
        MarioState();
        static Stage *stage;
};