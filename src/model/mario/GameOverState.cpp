#include "GameOverState.h"
#include "../Mario.hpp"

const GameOverState GameOverState::instance{};

const GameOverState *GameOverState::getInstance() {
    return &instance;
}

const MarioState *GameOverState::reset(Mario &) const {
    return this;
}

bool GameOverState::getIsLevelCompleted() const {
    return true;
};

bool GameOverState::getIsGameOver() const {
    return true;
}

const MarioState *GameOverState::update(Mario &mario) const {
    mario.estado = GAME_OVER;
    return this;
}