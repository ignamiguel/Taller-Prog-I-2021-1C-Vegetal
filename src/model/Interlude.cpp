#include "Interlude.h"
#include "../utils/exitStatus.h"

Interlude::Interlude(bool gameOver, int sceneNumber) : Scene() {
    estadoNivel.sceneNumber = sceneNumber;
    if (gameOver) {
        estadoNivel.exitStatus = CLIENT_GAME_OVER;
    } else {
        estadoNivel.exitStatus = sceneNumber == 4 ? CLIENT_GAME_COMPLETE : CLIENT_CONNECTION_CLOSED;
    }
};