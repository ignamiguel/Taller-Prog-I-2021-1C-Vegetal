#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "mario/SueloState.h"
#include "mario/MuriendoState.h"

#define MARIO_X_DIF 3.f
#define MARIO_Y_DIF 2.f

Mario::Mario() : Entidad(0, 0, ANCHO_MARIO, ALTO_MARIO), state(SueloState::getInstance()) {}

void Mario::reset() {
    this->state = this->state->reset(*this);
}

void Mario::setStageAndReset(Stage *stage) {
    MarioState::setStage(stage);
    this->reset();
}

void Mario::mover() {
    this->state = this->state->update(*this);
    controls.space = 0;
}

void Mario::die() {
    if (contador != 0) return;
    ++contador;
    --lives;
    this->audioObserver.update(DEATH);
    this->state = MuriendoState::getInstance();
}

estadoMario_t Mario::getEstado() const {
    return {pos, estado, isEnabled, this->audioObserver.getState()};
}

dimensiones_t Mario::dimensions() const {
    return {pos.x + MARIO_X_DIF, pos.y + MARIO_Y_DIF, pos.x + (ANCHO_MARIO - MARIO_X_DIF), pos.y + (ALTO_MARIO - MARIO_Y_DIF)};
}

unsigned char Mario::getScore() {
    return this->score;
}

void Mario::addPoints(unsigned char points) {
    this->score += points;
}

bool Mario::getIsLevelCompletedOrDisabled() {
    return (this->state->getIsLevelCompleted() || !this->isEnabled);
}

bool Mario::getIsGameOver() {
    return this->state->getIsGameOver();
}
