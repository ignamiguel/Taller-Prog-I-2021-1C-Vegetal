#include "Nivel1.h"
#include "../configuration.hpp"
#include "../logger.h"
#include "../utils/Constants.hpp"

Nivel1::Nivel1() : Nivel() {
    this->initPlatforms();
    auto config = configuration::GameConfiguration::getInstance(CONFIG_FILE);
    auto configEnemies = config->getEnemies();
    for (auto enemy: configEnemies) {
        if (enemy.getType().compare("Fuego") == 0) this->addEnemies(enemy.getQuantity());
        logger::Logger::getInstance().logDebug("Enemy type: " + enemy.getType());
        logger::Logger::getInstance().logDebug("Enemy quantity: " + std::to_string(enemy.getQuantity()));
    }
    this->initLadders();
    estadoNivel->level = 1;
}

void Nivel1::initPlatforms() {
    for (unsigned int i = 0; i < 12; ++i) {
        movingPlatforms[i] = new MovingPlatform(i % 3, i / 3);
        this->stage->addPlatform(movingPlatforms[i]);
    }

    platforms.push_back(new Platform(0, 248, 48, 248));
    platforms.push_back(new Platform(64, 248, 88, 248));
    platforms.push_back(new Platform(104, 248, 128, 248));
    platforms.push_back(new Platform(144, 248, 168, 248));
    platforms.push_back(new Platform(184, 248, 224, 248));
    platforms.push_back(new Platform(0, 200, 48, 200));
    platforms.push_back(new Platform(64, 200, 88, 200));
    platforms.push_back(new Platform(104, 200, 128, 200));
    platforms.push_back(new Platform(144, 200, 168, 200));
    platforms.push_back(new Platform(184, 200, 224, 200));
    platforms.push_back(new Platform(0, 160, 16, 160));
    platforms.push_back(new Platform(208, 160, 224, 160));
    platforms.push_back(new Platform(0, 120, 16, 120));
    platforms.push_back(new Platform(208, 120, 224, 120));
    platforms.push_back(new Platform(208, 88, 224, 88));
    platforms.push_back(new Platform(160, 87, 192, 87));
    platforms.push_back(new Platform(24, 84, 144, 84));

    for (Platform *platform : platforms) stage->addPlatform(platform);
}

void Nivel1::initLadders() {
    stage->addLadder(new Ladder(28, 232, 184));
    stage->addLadder(new Ladder(212, 184, 144));
    stage->addLadder(new Ladder(-4, 144, 104));
    stage->addLadder(new Ladder(212, 104, 72));
    stage->addLadder(new Ladder(124, 68, 40));
}

void Nivel1::addPlayers(std::vector<Mario *> *players) {
    this->players = players;
    for (Mario *player : *players) {
        player->setStage(stage);
        player->setPos(MARIO_START_X, MARIO_START_Y);
    }
}

void Nivel1::addEnemies(unsigned int amount) {
    for (unsigned int i = 0; i < amount; ++i) {
        unsigned int j = 1 + (rand() % (platforms.size() - 1));           // Omite plataforma inicial
        Platform *platform = platforms[j];
        punto_t pos = platform->getRandomPoint(ANCHO_ENEMIGO_FUEGO);
        pos.y -= ALTO_ENEMIGO_FUEGO;
        int direccion = (rand() % 2) ? -1 : 1;
        EnemigoFuego *enemy = new EnemigoFuego(pos, direccion);
        float min, max;
        platform->getLimits(&min, &max);
        enemy->setLimites(min, max);
        this->enemies.push_back(enemy);
    }
}

void Nivel1::update() {
    for (MovingPlatform *platform : movingPlatforms) platform->move();
    for (Mario *mario : *players) mario->mover();
    for (EnemigoFuego *enemy : enemies) enemy->mover();
}

estadoNivel_t* Nivel1::getEstado() {
    unsigned int i;
    for (i = 0; i < 12; ++i) {
        estadoNivel->platforms[i] = movingPlatforms[i]->getPos();
    }
    i = 0;
    for (EnemigoFuego *enemy : enemies) {
        estadoNivel->enemies[i++] = enemy->getPos();
    }
    i = 0;
    for (Mario *player : *players) {
        estadoNivel->players[i++] = player->getEstado();
    }
    
    return estadoNivel;
}

Nivel1::~Nivel1() {
    for (MovingPlatform *platform : movingPlatforms) delete platform;
}