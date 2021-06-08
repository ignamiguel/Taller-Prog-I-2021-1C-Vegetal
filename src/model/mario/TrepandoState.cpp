#include "../Mario.hpp"
#include "TrepandoState.h"
#include "TrepandoReposoState.h"
#include "CorriendoState.h"
#include "SaltandoState.h"
#include "ReposoState.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include "../../utils/Constants.hpp"

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

MarioState* TrepandoState::handleInput(char controls, Mario* mario) {
    char up = (controls & UP) != 0;
    char down = (controls & DOWN) != 0;

    // Mario esta al principio de la escalera ?
    if (mario->getPos().y > this->e->getY0()) {
        mario->setPos(mario->getPos().x, this->e->getY0());
        return ReposoState::getInstance();
    }

    // Mario esta al final de la escalera ?
    if (mario->getPos().y < this->e->getY1()) {
        mario->setPos(mario->getPos().x, this->e->getY1());
        return ReposoState::getInstance();
    }

    if (up || down) {
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
    return TREPANDO;
}