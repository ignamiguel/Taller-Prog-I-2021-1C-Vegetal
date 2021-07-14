#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "mario/SueloState.h"
#include "mario/MuriendoState.h"

Mario::Mario() : Entidad(0, 0, ANCHO_MARIO, ALTO_MARIO), state(SueloState::getInstance()) {}

void Mario::reset() {
    this->state = this->state->reset(*this);
}

void Mario::setStage(const Stage *stage) const {
    MarioState::setStage(stage);
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

unsigned int Mario::getScore() {
    return this->score;
}

void Mario::addScore(unsigned int points) {
    this->score += points;
}