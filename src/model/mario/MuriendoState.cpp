#include "MuriendoState.h"
#include "SueloState.h"
#include "GameOverState.h"
#include "../../utils/Constants.hpp"
#include <iostream>


#define PASAR_A_MURIENDO_ESTADO_2 20
#define PASAR_A_MURIENDO_ESTADO_3 40
#define PASAR_A_ESTADO_SUELO 60

MuriendoState *MuriendoState::instance = NULL;

MuriendoState::MuriendoState () {
}

MuriendoState  *MuriendoState::getInstance() {
    if (instance == NULL) {
        instance = new MuriendoState ();
    }
    return instance;
}

MarioState *MuriendoState::update(Mario *mario) {
    switch (mario->estado)
    {
        case MURIENDO_1:
            mario->contador = mario->contador + 1;
            if(mario->contador == PASAR_A_MURIENDO_ESTADO_2)
                mario->estado = MURIENDO_2;
            break;
        case MURIENDO_2:
            mario->contador = mario->contador + 1;
            if(mario->contador == PASAR_A_MURIENDO_ESTADO_3)
                mario->estado = MURIENDO_3;
            break;
        case MURIENDO_3:
            mario->contador = mario->contador + 1;
            if(mario->contador == PASAR_A_ESTADO_SUELO){
                mario->reset();
                if(mario->isGameOver()) {
                    return GameOverState::getInstance();
                }
                return SueloState::getInstance();
            }
            break;
        default:
            mario->contador = 0;
            mario->estado = MURIENDO_1;
            std::cout << "default " << MURIENDO_1 << "=" << mario->estado << "?" << std::endl;
            break;
    }
    return instance;
}