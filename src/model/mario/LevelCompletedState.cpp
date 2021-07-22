#include "LevelCompletedState.h"
#include "../Mario.hpp"

const LevelCompletedState LevelCompletedState::instance{};

const LevelCompletedState *LevelCompletedState::getInstance() {
    return &instance;
}

bool LevelCompletedState::getIsLevelCompleted() const {
    return true;
};

const MarioState *LevelCompletedState::update(Mario &mario) const {
    mario.estado = REPOSO;
    return this;
}