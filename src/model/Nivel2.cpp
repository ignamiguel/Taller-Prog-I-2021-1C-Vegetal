#include "Nivel2.h"
#include "../utils/Constants.hpp"

Nivel2::Nivel2() : Nivel() {
    this->initPlatforms();
    this->initLadders();
    estadoNivel.level = 2;
}

void Nivel2::initPlatforms() {
    platforms.emplace_back(0.f, 248.f, 120.f, 248.f);
    platforms.emplace_back(112.f, 248.f, 224.f, 241.f);
    platforms.emplace_back(0.f, 208.f, 208.f, 221.f);
    platforms.emplace_back(16.f, 188.f, 224.f, 175.f);
    platforms.emplace_back(0.f, 142.f, 208.f, 155.f);
    platforms.emplace_back(16.f, 122.f, 224.f, 109.f);
    platforms.emplace_back(136.f, 84.5f, 208.f, 89.f);
    platforms.emplace_back(0.f, 84.f, 130.f, 84.f);
    platforms.emplace_back(88.f, 56.f, 135.f, 56.f); // top

    for (auto &platform : platforms) stage.addPlatform(&platform);
}

void Nivel2::initLadders() {
    stage.addLadder({180, 227.75f, 203.25f});
    stage.addLadder({92, 197.75f, 167.25f});
    stage.addLadder({28, 193.75f, 171.25f});
    stage.addLadder({108, 166.25f, 132.75f});
    stage.addLadder({180, 161.75f, 137.25f});
    stage.addLadder({68, 130.25f, 102.75f});
    stage.addLadder({28, 127.75f, 105.25f});
    stage.addLadder({180, 95.75f, 71.25f});
    stage.addLadder({124, 68, 40});
}

void Nivel2::update() {
    if (++tick % 128 == 0) addBarrel();

    this->updateBarrels();
    for (auto &mario : *players) mario.mover();
}

void Nivel2::addBarrel() {
    const float x = rand() % (ANCHO_NIVEL - ANCHO_BARRIL);
    this->barriles.emplace_back(x, (float)N2_POS_Y_BARRIL);
}

void Nivel2::updateBarrels() {
    for (auto it = barriles.begin(); it != barriles.end();) {
        it->mover();
        if (it->estaEnNivel()) {
            ++it;
        } else {
            it = this->barriles.erase(it);
        }
    }
}

const estadoNivel_t &Nivel2::getEstado() {
    size_t i = 0;
    for (auto &barril : barriles) {
        estadoNivel.barrels[i++] = barril.pos;
    }
    estadoNivel.barrels[i] = {0, 0};
    i = 0;
    for (auto &player : *players) {
        estadoNivel.players[i++] = player.getEstado();
    }

    return estadoNivel;
}