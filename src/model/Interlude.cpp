#include "Interlude.h"
#include "../utils/exitStatus.h"

Interlude::Interlude(bool gameOver, int sceneNumber) : Scene() {
    estadoNivel.sceneNumber = sceneNumber;
    estadoNivel.gameOver = gameOver || sceneNumber == 4;
};