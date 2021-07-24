#include "SueloState.h"
#include "AireState.h"
#include "TrepandoState.h"
#include "../Mario.hpp"
#include "../../utils/Constants.hpp"

const SueloState SueloState::instance{};

const SueloState *SueloState::getInstance() {
    return &instance;
}

const MarioState *SueloState::update(Mario &mario) const {
    const Ladder *ladder = stage->getLadder(mario.pos.x, mario.pos.y, mario.controls.up - mario.controls.down);
    if (ladder != nullptr) {
        const float distance = ladder->x - mario.pos.x;
        if (-4 <= distance && distance <= 4) {
            mario.pos.x = ladder->x;
            mario.pos.y -= (mario.controls.up - mario.controls.down) * MARIO_VEL_TREPAR;
            mario.climbMin = ladder->bottom;
            mario.climbMax = ladder->top;
            return TrepandoState::getInstance();
        }
    }
    mario.velX = (mario.controls.right - mario.controls.left) * MARIO_VEL_X;
    mario.velY = GRAVEDAD;
    mario.pos.x += mario.velX * ((mario.pos.x < ANCHO_NIVEL - ANCHO_MARIO && 0 < mario.velX) || (0 < mario.pos.x && mario.velX < 0));
    mario.pos.y -= mario.velY;
    if (mario.controls.space) {
        mario.velY = MARIO_VEL_SALTO;
        mario.audioObserver.update(JUMP);
        return AireState::getInstance();
    }
    if (!stage->collide(mario.pos.x, mario.pos.y, mario.velX, mario.velY)) {
        mario.velX = 0;
        mario.velY = 0;
        return AireState::getInstance();
    }
    
    if(mario.velX > 0) {
        mario.direccion = DERECHA;
    } else if (mario.velX < 0){
        mario.direccion = IZQUIERDA;
    }
    
    mario.estado = (mario.estado == DE_ESPALDAS) + (mario.velX != 0) * (CORRIENDO - (mario.estado == DE_ESPALDAS));
    return this;
}
