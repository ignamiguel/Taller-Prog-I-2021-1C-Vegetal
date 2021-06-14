#include "../Mario.hpp"
#include "CorriendoState.h"
#include "SaltandoState.h"
#include "ReposoState.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include "../../utils/Constants.hpp"

#define MARIO_VELOCIDAD 0.5

CorriendoState* CorriendoState::instance;

CorriendoState::CorriendoState() {
    this->name = "Corriendo";
}

CorriendoState* CorriendoState::getInstance() {
    if (CorriendoState::instance == NULL) {
        CorriendoState::instance = new CorriendoState();
    }

    return CorriendoState::instance;
}

void CorriendoState::setDir(char left, char right){
    this->left = left;
    this->right = right;
};

MarioState* CorriendoState::handleInput(char controls, Mario* mario) {
    char left = (controls & LEFT) != 0;
    char right = (controls & RIGHT) != 0;
    char space = (controls & SPACE) != 0;

    if(space) {
        SaltandoState* saltandoState = SaltandoState::getInstance();
        saltandoState->setDir(left, right, this->velX);
        return saltandoState;
    }

    if(left != right) {
        CorriendoState* corriendoState = CorriendoState::getInstance();
        corriendoState->setDir(left, right);
        return corriendoState;
    }

    return ReposoState::getInstance();
}

void CorriendoState::perform() {
    this->velX = (this->right - this->left) * MARIO_VELOCIDAD;
}

void CorriendoState::update() {}

char CorriendoState::getEstado() {
    return CORRIENDO;
}