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

    directionalPoints.push_front((directionalPoint_t){136.f, 84 - ALTO_BARRIL, 0.935f, 0.065f});
    directionalPoints.push_front((directionalPoint_t){208.f, 89 - ALTO_BARRIL, 0.f, 1.f});
    directionalPoints.push_front((directionalPoint_t){208.f, 110 - ALTO_BARRIL, -0.941f, 0.059f});
    directionalPoints.push_front((directionalPoint_t){16 - ANCHO_BARRIL, 122 - ALTO_BARRIL, 0.f, 1.f});
    directionalPoints.push_front((directionalPoint_t){16 - ANCHO_BARRIL, 143 - ALTO_BARRIL, 0.941f, 0.059f});
    directionalPoints.push_front((directionalPoint_t){208.f, 156 - ALTO_BARRIL, 0.f, 1.f});
    directionalPoints.push_front((directionalPoint_t){208.f, 176 - ALTO_BARRIL, -0.941f, 0.059f});
    directionalPoints.push_front((directionalPoint_t){16 - ANCHO_BARRIL, 189 - ALTO_BARRIL, 0.f, 1.f});
    directionalPoints.push_front((directionalPoint_t){16 - ANCHO_BARRIL, 209 - ALTO_BARRIL, 0.941f, 0.059f});
    directionalPoints.push_front((directionalPoint_t){208.f, 222 - ALTO_BARRIL, 0.f, 1.f});
    directionalPoints.push_front((directionalPoint_t){208.f, 242 - ALTO_BARRIL, -0.941f, 0.059f});
    directionalPoints.push_front((directionalPoint_t){112, 248 - ALTO_BARRIL, -1.f, 0.f});
    
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

void Nivel2::addPlayers(std::vector<Mario> &players) {
    this->players = &players;
    for (auto &player : players) {
        player.setStage(&stage);
        player.reset();
    }
}

void Nivel2::update() {
    if (++tick % 128 == 0) addBarrel();

    this->updateBarrels();
    for (auto &mario : *players) mario.mover();
}

void Nivel2::addBarrel() {
    this->barriles.emplace_back((float)N2_POS_X_BARRIL, (float)N2_POS_Y_BARRIL);
}

void Nivel2::updateBarrels() {
    for (auto it = barriles.begin(); it != barriles.end();) {
        updateBarrelDirection(&(*it));
        (*it).mover();
        if (!it->estaEnNivel()) {
            it = this->barriles.erase(it);
        } else ++it;
    }
}

void Nivel2::updateBarrelDirection(Barril* barril) {
    for (directionalPoint_t directionalPoint : directionalPoints) {
        if ((int)barril->pos.x == (int)directionalPoint.x && (int)barril->pos.y == (int)directionalPoint.y) {
            barril->updateDirection(directionalPoint.xDirection,directionalPoint.yDirection);
            break;
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

Nivel2::~Nivel2() {
    barriles.clear();
}