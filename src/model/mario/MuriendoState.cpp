#include "MuriendoState.h"
#include "GameOverState.h"
#include "SueloState.h"
#include "../Mario.hpp"

#define TIEMPO_MURIENDO 96
#define TIEMPO_RESPAWN 128

const MuriendoState MuriendoState::instance{};

const MuriendoState *MuriendoState::getInstance() {
    return &instance;
}

const MarioState *MuriendoState::update(Mario &mario) const {
    ++mario.contador;
    if (mario.contador > TIEMPO_RESPAWN) {
        if (mario.lives == 0) {
            return GameOverState::getInstance();
        }
        mario.contador = 0;
        mario.reset();
        return SueloState::getInstance();
    }
    mario.estado = MURIENDO;
    if (mario.contador > TIEMPO_MURIENDO) {
        mario.estado = MUERTO;
    }
    return this;
}