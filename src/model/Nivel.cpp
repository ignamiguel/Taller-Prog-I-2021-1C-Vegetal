#include "Nivel.h"

Nivel::Nivel() {
    estadoNivel = new estadoNivel_t();
    stage = new Stage();
}

bool Nivel::isComplete() {
    bool result = false;
    for (Mario *mario : *players) result |= (mario->getPos().y <= 40.5);
    return result;
}

Nivel::~Nivel() {
    delete estadoNivel;

    delete stage;

    for (auto platform : platforms) delete platform;
    platforms.clear();

    for (auto enemy : enemies) delete enemy;
    enemies.clear();
}