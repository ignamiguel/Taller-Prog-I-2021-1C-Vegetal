#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "mario/SueloState.h"

const int MARIO_ANCHO_REAL = 14;

Mario::Mario() : Entidad(0, 0, ANCHO_MARIO, ALTO_MARIO, MARIO_ANCHO_REAL, ALTO_MARIO) {
    this->state = SueloState::getInstance();
    this->isEnabled = true;
    this->lives = 3;
}

void Mario::setControls(controls_t controls) {
    this->controls = controls;
}

void Mario::setPos(float x, float y) {
    this->posX = x;
    this->posY = y;
}

void Mario::setStage(Stage *stage) {
    this->state->setStage(stage);
    this->state = SueloState::getInstance();
}

void Mario::mover() {
    this->state = this->state->update(&posX, &posY, &velX, &velY, &estado, controls);
}

estadoMario_t Mario::getEstado() {
    estadoMario_t estadoMario;
    estadoMario.pos = getPos();
    if(this->isEnabled) {
        estadoMario.estado = this->estado;
    } else {
        estadoMario.estado = DESCONECTADO;
    }
    return estadoMario;
}

void Mario::disable() {
    this->isEnabled = false;
}

void Mario::enable() {
    this->isEnabled = true;
}

void Mario::die() {
    this->lives--;
    this->reset();
    if (this->lives == 0) {
        this->disable();
    }
}

void Mario::reset() {
    this->posX = MARIO_START_X;
    this->posY = MARIO_START_Y;
    this->velX = 0;
    this->velY = 0;
    this->state = SueloState::getInstance();
} 