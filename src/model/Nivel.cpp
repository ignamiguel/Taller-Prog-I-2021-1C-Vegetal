#include "Nivel.h"

bool Nivel::isComplete() const {
    bool result = false;
    for (auto &mario : *players) result |= (mario.pos.y <= 40);
    return result;
}

Nivel::~Nivel() {
    enemies.clear();
    platforms.clear();
}

bool Nivel::collision(dimensiones_t player, dimensiones_t enemy) {

    bool collisionX = (enemy.x1 >= player.x1 &&  enemy.x1 <= player.x2) || (enemy.x2 >= player.x1 &&  enemy.x2 <= player.x2);
    bool collisionY = (enemy.y1 >= player.y1 &&  enemy.y1 <= player.y2) || (enemy.y2 >= player.y1 &&  enemy.y2 <= player.y2);

    return (collisionX && collisionY);
}