#include "AireState.h"
#include "SueloState.h"
#include "MuriendoState.h"
#include "../Mario.hpp"
#include "../../utils/Constants.hpp"

const AireState AireState::instance{};

const AireState *AireState::getInstance() {
    return &instance;
}

const MarioState *AireState::update(Mario &mario) const {
    mario.estado += (SALTANDO - mario.estado) * (mario.velY > 0);
    mario.velX -= mario.velX * 2 * !((mario.pos.x < ANCHO_NIVEL - ANCHO_MARIO && 0 < mario.velX) || (0 < mario.pos.x && mario.velX < 0));
    mario.velY += GRAVEDAD;
    mario.pos.x += mario.velX;
    mario.pos.y -= mario.velY;
    if (stage->collide(mario.pos.x, mario.pos.y, mario.velX, mario.velY)) return SueloState::getInstance();
    if (mario.pos.y > ALTO_NIVEL - ALTO_MARIO) {
        if(mario.collider->getType() == TEST_MODE) {
            mario.pos.y = ALTO_NIVEL - ALTO_MARIO;
            mario.estado = CORRIENDO;
            return SueloState::getInstance();
        } else {
            mario.die();
            return MuriendoState::getInstance();
        }
    }
    return this;
}