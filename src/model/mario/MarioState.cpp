#include "MarioState.h"
#include <string>

#include <stdio.h>
#include <iostream>

MarioState::MarioState() {
    this->velX = 0.;
    this->velY = 0.;
}

std::string MarioState::getName() {
    return this->name;
}

float MarioState::getVelY() {
    return this->velY;
}

float MarioState::getVelX() {
    return this->velX;
}

void MarioState::actualizarPlataforma(Mario* mario) {
    Plataforma* p = mario->getNivel()->getPlataforma(mario->getPos());
    if (p == NULL) {
        std::cout << "no hay plataforma en esa pos " << mario->getPos().y << std::endl;
    } else {
        mario->setPlataformaActual(p);
    }
}
