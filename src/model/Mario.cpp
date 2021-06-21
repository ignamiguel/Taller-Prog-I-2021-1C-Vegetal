#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "mario/SueloState.h"

Mario::Mario() : Entidad(0, 0, ANCHO_MARIO, ALTO_MARIO) {
    this->state = SueloState::getInstance();
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
    estado = REPOSO;
}

void Mario::mover() {
    this->state = this->state->update(&posX, &posY, &velX, &velY, &estado, controls);
}

estadoMario_t Mario::getEstado() {
    estadoMario_t estadoMario;
    estadoMario.pos = getPos();
    estadoMario.estado = estado;
    return estadoMario;
}