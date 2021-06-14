#include "../Mario.hpp"
#include "ReposoState.h"
#include "CorriendoState.h"
#include "SaltandoState.h"
#include "TrepandoState.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include "../../utils/Constants.hpp"

#define MARIO_VELOCIDAD_EN_REPOSO 0

ReposoState* ReposoState::instance;

ReposoState::ReposoState() : MarioState() {
    this->name = "Reposo";
}

ReposoState* ReposoState::getInstance() {
    if (ReposoState::instance == NULL) {
        ReposoState::instance = new ReposoState();
    }

    return ReposoState::instance;
}

MarioState* ReposoState::handleInput(char controls, Mario* mario) {
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
            return ReposoState::getInstance();
        }

        // Si Mario esta en el extremo inferior de la escalera
        // y presiona para abajo, no cambia el estado
        if( mario->getPos().y == (e->getY0() - ALTO_MARIO)
            && down) {
             return ReposoState::getInstance();
        }

        // Si Mario esta en el extremo superior de la escalera
        // y presiona para arriba, no cambia el estado
        if( mario->getPos().y == (e->getY1() - ALTO_MARIO)
            && up) {
             return ReposoState::getInstance();
        }

        mario->setPos(e->getCenter(), mario->getPos().y);
        TrepandoState* trepandoState = TrepandoState::getInstance();
        trepandoState->setDir(up, down);
        trepandoState->setEscalera(e);
        return trepandoState;
    }
 
    return ReposoState::getInstance();
}

void ReposoState::perform() {
}

void ReposoState::update() {  
}

char ReposoState::getEstado() {
    return REPOSO;
}