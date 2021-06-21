#include "AireState.h"
#include "SueloState.h"
#include "../../utils/Constants.hpp"

AireState *AireState::instance = NULL;

AireState::AireState() {}

AireState *AireState::getInstance() {
    if (instance == NULL) {
        instance = new AireState();
    }
    return instance;
}

MarioState *AireState::update(float *x, float *y, float *xSpeed, float *ySpeed, char *estado, controls_t) {
    *estado += (SALTANDO - *estado) * (*ySpeed > 0);
    *xSpeed -= *xSpeed * 2 * !((*x < ANCHO_NIVEL - ANCHO_MARIO && 0 < *xSpeed) || (0 < *x && *xSpeed < 0));
    *ySpeed += GRAVEDAD;
    *x += *xSpeed;
    *y -= *ySpeed;
    if (stage->collide(x, y, xSpeed, ySpeed)) return SueloState::getInstance();
    if (*y > ALTO_NIVEL - ALTO_MARIO) {             // Aca moriria mario
        *x = MARIO_START_X;
        *y = MARIO_START_Y;
        *xSpeed = 0;
        *ySpeed = 0;
        return SueloState::getInstance();
    }
    return instance;
}