#include "../Mario.hpp"
#include "TrepandoState.h"
#include "TrepandoReposoState.h"
#include "TrepandoFinalState.h"
#include "CorriendoState.h"
#include "SaltandoState.h"
#include "ReposoState.h"
#include <string>
#include <stdio.h>
#include <iostream>

#define MARIO_VELOCIDAD 0.5

TrepandoState* TrepandoState::instance;

TrepandoState::TrepandoState() {
    this->name = "Trepando";
}

TrepandoState* TrepandoState::getInstance() {
    if (TrepandoState::instance == NULL) {
        TrepandoState::instance = new TrepandoState();
    }

    return TrepandoState::instance;
}

void TrepandoState::setDir(char up, char down){
    this->up = up;
    this->down = down;
};

void TrepandoState::setEscalera(Escalera* e) {
    this->e = e;
}

void TrepandoState::clear() {
    this->e = NULL;
}

MarioState* TrepandoState::handleInput(char controls, Mario* mario) {
    int a = this->e->getY0() - mario->getPos().y;
    int b = (a / 4);
    int c = b % 2;
    if (c == 0) {
        this->estado = TREPANDO_0;
    } else {
        this->estado = TREPANDO_1;
    }
    
    char up = (controls & UP) != 0;
    char down = (controls & DOWN) != 0;

    // Mario esta al principio de la escalera ?
    if (mario->getPos().y > (this->e->getY0() - ALTO_MARIO)) {
        mario->setPos(mario->getPos().x, (this->e->getY0() - ALTO_MARIO));
        this->clear();
        return TrepandoFinalState::getInstance();
    }

    // Mario esta al final de la escalera ?
    if (mario->getPos().y < (this->e->getY1() - ALTO_MARIO)) {
        mario->setPos(mario->getPos().x, (this->e->getY1() - ALTO_MARIO));
        this->clear();
        return TrepandoFinalState::getInstance();
    }

    if (up != down) {
        TrepandoState* trepandoState = TrepandoState::getInstance();
        trepandoState->setDir(up, down);
        return trepandoState;
    }

    TrepandoReposoState* trepandoReposoState = TrepandoReposoState::getInstance();
    trepandoReposoState->setEscalera(this->e);
    return trepandoReposoState;
}

void TrepandoState::perform() {
    this->velY = (this->up - this->down) * MARIO_VELOCIDAD;
}

void TrepandoState::update() {}

char TrepandoState::getEstado() {
    return this->estado;
}