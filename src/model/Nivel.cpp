#include "Nivel.h"


Nivel::Nivel() {
    estadoNivel = new estadoNivel_t();
    stage = new Stage();
}

bool Nivel::isComplete() {
    bool result = false;
    for (Mario *mario : *players) result |= (mario->getPos().y == 40);
    return result;
}

bool Nivel::collision(SDL_FRect player, SDL_FRect enemy) {
     
    bool colisionPlayerX = (player.x > enemy.x &&  player.x < enemy.w) || (player.w > enemy.x &&  player.w < enemy.w);
    bool colisionPlayerY = (player.y > enemy.y &&  player.y < enemy.h) || (player.h > enemy.y &&  player.h < enemy.h);

    bool colisionEnemyX = (enemy.x > player.x &&  enemy.x < player.w) || (enemy.w > player.x &&  enemy.w < player.w);
    bool colisionEnemyY = (enemy.y > player.y &&  enemy.y < player.h) || (enemy.h > player.y &&  enemy.h < player.h);

    return ((colisionPlayerX && colisionPlayerY) || (colisionEnemyX && colisionEnemyY));
}

void Nivel::checkCollisions () {

    for (Mario *player : *players) {
        
        for (Entidad *enemy : enemies) {
            
            if (collision (player->dimensions(), enemy->dimensions())) {
                player->die();
                break;
            }
        }
    }
}

Nivel::~Nivel() {
    delete estadoNivel;

    delete stage;

    for (auto platform : platforms) delete platform;
    platforms.clear();

    for (auto enemy : enemies) delete enemy;
    enemies.clear();
}