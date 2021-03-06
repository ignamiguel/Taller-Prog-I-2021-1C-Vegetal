#include "Nivel.h"

Nivel::Nivel(std::vector<Mario> *players) : Scene() {
    this->players = players;
    for (auto &player : *players) {
        player.setStageAndReset(&stage);
    }
}

bool Nivel::collision(dimensiones_t player, dimensiones_t enemy) const {
    return enemy.x1 < player.x2 && player.x1 < enemy.x2 &&
           enemy.y1 < player.y2 && player.y1 < enemy.y2;
}

bool Nivel::isComplete() const {
    bool allMariosHaveCompletedTheLevel = true;
    for (auto &mario : *players) {
        allMariosHaveCompletedTheLevel &= mario.getIsLevelCompletedOrDisabled();
    }
    return allMariosHaveCompletedTheLevel;
}