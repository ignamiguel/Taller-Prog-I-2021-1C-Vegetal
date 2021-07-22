#include "Nivel1.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../utils/Constants.hpp"

Nivel1::Nivel1() : Nivel() {
    this->initPlatforms();
    auto config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    auto configEnemies = config->getEnemies();
    for (auto &enemy: configEnemies) {
        if (enemy.getType().compare("Fuego") == 0)
            this->addEnemies(enemy.getQuantity());
        logger::Logger::getInstance().logDebug("Enemy type: " + enemy.getType());
        logger::Logger::getInstance().logDebug("Enemy quantity: " + std::to_string(enemy.getQuantity()));
    }
    this->initLadders();
    this->initHammers();
    estadoNivel.level = 1;
}

void Nivel1::initPlatforms() {
    for (auto &platform : movingPlatforms) {
        stage.addPlatform(&platform);
    }

    platforms.emplace_back(64.f, 248.f, 88.f, 248.f);
    platforms.emplace_back(104.f, 248.f, 128.f, 248.f);
    platforms.emplace_back(144.f, 248.f, 168.f, 248.f);
    platforms.emplace_back(184.f, 248.f, 224.f, 248.f);
    platforms.emplace_back(0.f, 200.f, 48.f, 200.f);
    platforms.emplace_back(64.f, 200.f, 88.f, 200.f);
    platforms.emplace_back(104.f, 200.f, 128.f, 200.f);
    platforms.emplace_back(144.f, 200.f, 168.f, 200.f);
    platforms.emplace_back(184.f, 200.f, 224.f, 200.f);
    platforms.emplace_back(160.f, 87.f, 192.f, 87.f);
    platforms.emplace_back(24.f, 84.f, 144.f, 84.f);
    platforms.emplace_back(0.f, 160.f, 16.f, 160.f);
    platforms.emplace_back(208.f, 160.f, 224.f, 160.f);
    platforms.emplace_back(0.f, 120.f, 16.f, 120.f);
    platforms.emplace_back(208.f, 120.f, 224.f, 120.f);
    platforms.emplace_back(208.f, 88.f, 224.f, 88.f);
    platforms.emplace_back(0.f, 248.f, 48.f, 248.f); // bottom
    platforms.emplace_back(88.f, 56.f, 135.f, 56.f); // top

    for (auto &platform : platforms) stage.addPlatform(&platform);
}

void Nivel1::initLadders() {
    stage.addLadder({28, 232, 184});
    stage.addLadder({212, 184, 144});
    stage.addLadder({-4, 144, 104});
    stage.addLadder({212, 104, 72});
    stage.addLadder({124, 68, 40});
}

void Nivel1::initHammers() {
    for (int i = 0; i < MAX_HAMMERS; ++i) {
        hammers.emplace_back(punto_t{48.f + rand() / (float)RAND_MAX * 160.f, 208.f});
    }
}

void Nivel1::addEnemies(unsigned int amount) {
    for (unsigned int i = 0; i < amount; ++i) {
        const unsigned int j = rand() % 11;           // Omite plataformas inicial y final
        const Platform &platform = platforms[j];
        punto_t pos = platform.getRandomPoint(ANCHO_ENEMIGO_FUEGO);
        pos.y -= ALTO_ENEMIGO_FUEGO;
        const int direccion = (rand() % 2) ? -1 : 1;
        float min, max;
        platform.getLimits(min, max);
        this->enemies.emplace_back(pos, direccion, min, max);
    }
}

void Nivel1::update() {
    for (auto &platform : movingPlatforms) platform.move();
    for (auto &enemy : enemies) enemy.mover();
    for (auto &mario : *players) mario.mover();
    checkCollisions();
}

const estadoNivel_t &Nivel1::getEstado() {
    size_t i = 0;
    for (; i < 12; ++i) {
        estadoNivel.platforms[i] = movingPlatforms[i].getPos();
    }
    i = 0;
    for (auto &enemy : enemies) {
        estadoNivel.enemies[i++] = enemy.pos;
    }
    i = 0;
    for (auto &hammer : hammers) {
        estadoNivel.hammers[i++] = hammer.pos;
    }
    if (i < MAX_HAMMERS) estadoNivel.hammers[i] = {0, 0};
    i = 0;
    for (auto &player : *players) {
        estadoNivel.players[i++] = player.getEstado();
    }

    estadoNivel.isGameOver = this->getIsGameOver();

    return estadoNivel;
}

void Nivel1::checkCollisions() const {
    for (Mario &player : *players) {
        for (auto &enemy : enemies) {
            if (collision(player.dimensions(), enemy.dimensions())) {
                player.die();
                break;
            }
        }
    }
}