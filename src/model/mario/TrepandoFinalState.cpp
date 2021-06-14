#include "../Mario.hpp"
#include "TrepandoFinalState.h"
#include "ReposoState.h"
#include "CorriendoState.h"
#include "SaltandoState.h"
#include "TrepandoState.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include "../../utils/Constants.hpp"

#include <stdio.h>
#include <iostream>

#define MARIO_VELOCIDAD_EN_REPOSO 0

TrepandoFinalState* TrepandoFinalState::instance;

TrepandoFinalState::TrepandoFinalState() : MarioState() {
    this->name = "Trepando Final";
}

TrepandoFinalState* TrepandoFinalState::getInstance() {
    if (TrepandoFinalState::instance == NULL) {
        TrepandoFinalState::instance = new TrepandoFinalState();
    }

    return TrepandoFinalState::instance;
}

MarioState* TrepandoFinalState::handleInput(char controls, Mario* mario) {
    char left = (controls & LEFT) != 0;
    char right = (controls & RIGHT) != 0;
    char space = (controls & SPACE) != 0;

    char up = (controls & UP) != 0;
    char down = (controls & DOWN) != 0;

    if (left != right) {
        CorriendoState* state = CorriendoState::getInstance();
        state->setDir(left, right);
        return state;
    }

    if (space) {
        SaltandoState* saltandoState = SaltandoState::getInstance();
        saltandoState->setDir(left, right, MARIO_VELOCIDAD_EN_REPOSO);
        return saltandoState;
    }

    if (up != down) {
        Escalera* e = mario->getNivel()->getEscalera(mario->getPos());

        if (e == NULL) {
            std::cout << "no hay escaleras en esa plataforma " << mario->getPos().y << std::endl;
            return TrepandoFinalState::getInstance();
        }

        // Si Mario esta en el extremo inferior de la escalera
        // y presiona para abajo, no cambia el estado
        if( mario->getPos().y == (e->getY0() - ALTO_MARIO)
            && down) {
             return TrepandoFinalState::getInstance();
        }

        // Si Mario esta en el extremo superior de la escalera
        // y presiona para arriba, no cambia el estado
        if( mario->getPos().y == (e->getY1() - ALTO_MARIO)
            && up) {
             return TrepandoFinalState::getInstance();
        }

        mario->setPos(e->getCenter(), mario->getPos().y);
        TrepandoState* trepandoState = TrepandoState::getInstance();
        trepandoState->setDir(up, down);
        trepandoState->setEscalera(e);
        return trepandoState;
    }
 
    return TrepandoFinalState::getInstance();
}

void TrepandoFinalState::perform() {
}

void TrepandoFinalState::update() {  
}

char TrepandoFinalState::getEstado() {
    return TREPANDO_FINAL;
}