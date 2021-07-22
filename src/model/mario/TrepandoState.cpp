#include "SueloState.h"
#include "TrepandoState.h"
#include "LevelCompletedState.h"
#include "../Mario.hpp"

const TrepandoState TrepandoState::instance{};

const TrepandoState *TrepandoState::getInstance() {
    return &instance;
}

const MarioState *TrepandoState::update(Mario &mario) const {
    mario.velY = (mario.controls.up - mario.controls.down) * MARIO_VEL_TREPAR;
    if (mario.climbMin < mario.pos.y || mario.pos.y < mario.climbMax)
    {
        if (mario.pos.y <= TOP_LEVEL) {
            mario.addPoints(this->stage->getPointsForCompletingLevel());
            return LevelCompletedState::getInstance();
        }
        mario.estado = DE_ESPALDAS;
        return SueloState::getInstance();
    }
    mario.pos.y -= mario.velY;
    mario.estado = TREPANDO;
    return this;
}