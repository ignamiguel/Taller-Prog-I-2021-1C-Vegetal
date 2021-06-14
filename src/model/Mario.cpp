#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "mario/ReposoState.h"
#include "Nivel.h"
#include "Plataforma.h"

#define MAX_DESPLAZAMIENTO_X 228
#define MAX_DESPLAZAMIENTO_Y 232
#define MIN_DESPLAZAMIENTO_X -4

Mario::Mario() : Entidad(0, 0, ANCHO_MARIO, ALTO_MARIO) {
    this->state = ReposoState::getInstance();
}

void Mario::setNivel(Nivel* nivel) {
    this->nivel = nivel;
    this->plataformaActual = this->nivel->getPlataformaInicial();
}

Nivel* Mario::getNivel() {
    return this->nivel;
}

void Mario::mover() {
    this->state->update();
    float velY = this->state->getVelY();
    float velX = this->state->getVelX();

    this->posY -= velY;

    this->posX += velX;

    if ((this->posX < MIN_DESPLAZAMIENTO_X) 
        || ((this->posX + ANCHO_MARIO) > MAX_DESPLAZAMIENTO_X)) {
        this->posX -= velX;
    }
}

void Mario::setEstado(char controls) {
    this->state = this->state->handleInput(controls, this);
    this->state->perform();
}

void Mario::setPos(float x, float y) {
    this->posX = x;
    this->posY = y;
}

estadoMario_t Mario::getEstado() {
    estadoMario_t estadoMario;
    estadoMario.pos = getPos();
    estadoMario.estado = this->state->getEstado();
    return estadoMario;
}

Plataforma* Mario::getPlataformaActual() {
    return this->plataformaActual;
}

void Mario::setPlataformaActual(Plataforma* p) {
    this->plataformaActual = p;
}