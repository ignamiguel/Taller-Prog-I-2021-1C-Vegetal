#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "mario/SueloState.h"
#include "mario/MuriendoState.h"
#include <iostream>

const int MARIO_ANCHO_REAL = 14;

Mario::Mario() : Entidad(0, 0, ANCHO_MARIO, ALTO_MARIO) {
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
    this->state = this->state->update(this);
}

estadoMario_t Mario::getEstado() {
    estadoMario_t estadoMario;
    estadoMario.pos = getPos();
    estadoMario.lives = this->lives;
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

MarioState *Mario::die() {
    this->lives--;
    this->state = MuriendoState::getInstance();
    return MuriendoState::getInstance();;
}

void Mario::reset() {
    this->posX = MARIO_START_X;
    this->posY = MARIO_START_Y;
    this->velX = 0;
    this->velY = 0;
}

bool Mario::isGameOver() {
    return this->lives == 0;
}


SDL_FRect Mario::dimensions() {
    return {posX, posY, posX + MARIO_ANCHO_REAL, posY + ALTO_MARIO};
 }