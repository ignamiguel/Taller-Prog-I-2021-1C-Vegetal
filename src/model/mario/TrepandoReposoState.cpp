#include "../Mario.hpp"
#include "TrepandoReposoState.h"
#include "TrepandoFinalState.h"
#include "TrepandoState.h"
#include "CorriendoState.h"
#include "SaltandoState.h"
#include "ReposoState.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include "../../utils/Constants.hpp"

#define MARIO_VELOCIDAD 0.5

TrepandoReposoState* TrepandoReposoState::instance;

TrepandoReposoState::TrepandoReposoState() {
    this->name = "Trepando Reposo";
}

TrepandoReposoState* TrepandoReposoState::getInstance() {
    if (TrepandoReposoState::instance == NULL) {
        TrepandoReposoState::instance = new TrepandoReposoState();
    }

    return TrepandoReposoState::instance;
}

void TrepandoReposoState::setDir(char up, char down){
    this->up = up;
    this->down = down;
};

void TrepandoReposoState::setEscalera(Escalera* e) {
    this->e = e;
}

MarioState* TrepandoReposoState::handleInput(char controls, Mario* mario) {
    int a = this->e->getY0() - mario->getPos().y;
    int b = (a / 4);
    int c = b % 2;
    if (c == 0) {
        this->estado = TREPANDO_0;
    } else {
        this->estado = TREPANDO_1;
    }
    
    char left = (controls & LEFT) != 0;
    char right = (controls & RIGHT) != 0;
    char space = (controls & SPACE) != 0;
    
    char up = (controls & UP) != 0;
    char down = (controls & DOWN) != 0;

    // // Mario esta al principio de la escalera ?
    // if (mario->getPos().y > (this->e->getY0() - ALTO_MARIO)) {
    //     mario->setPos(mario->getPos().x, (this->e->getY0() - ALTO_MARIO));
    //     //this->clear();
    //     return TrepandoFinalState::getInstance();
    // }

    // // Mario esta al final de la escalera ?
    // if (mario->getPos().y < (this->e->getY1() - ALTO_MARIO)) {
    //     mario->setPos(mario->getPos().x, (this->e->getY1() - ALTO_MARIO));
    //     //this->clear();
    //     return TrepandoFinalState::getInstance();
    // }

    if (up != down) {
        TrepandoState* trepandoState = TrepandoState::getInstance();
        trepandoState->setDir(up, down);
        trepandoState->setEscalera(this->e);
        return trepandoState;
    }

    return TrepandoReposoState::getInstance();
}

void TrepandoReposoState::perform() {
}

void TrepandoReposoState::update() {}

char TrepandoReposoState::getEstado() {
    return this->estado;
}