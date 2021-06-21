#include "SueloState.h"
#include "TrepandoState.h"
#include "../../utils/Constants.hpp"

TrepandoState *TrepandoState::instance = NULL;

TrepandoState::TrepandoState() {}

TrepandoState *TrepandoState::getInstance() {
    if (instance == NULL) {
        instance = new TrepandoState();
    }
    return instance;
}

void TrepandoState::setLadder(Ladder *ladder) {
    this->ladder = ladder;
}

MarioState *TrepandoState::update(float *, float *y, float *, float *ySpeed, char *estado, controls_t controls) {
    *ySpeed = (controls.up - controls.down) * MARIO_VEL_TREPAR;
    if ((*ySpeed <= 0 && std::abs(ladder->getBottom() - *y) <= MARIO_VEL_TREPAR / 2)
    || (*ySpeed >= 0 && std::abs(ladder->getTop() - *y) <= MARIO_VEL_TREPAR / 2))
    {
        *estado = DE_ESPALDAS;
        return SueloState::getInstance();
    }
    *y -= *ySpeed;
    *estado = TREPANDO;
    return instance;
}