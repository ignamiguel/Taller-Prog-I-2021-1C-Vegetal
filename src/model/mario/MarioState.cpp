#include "MarioState.h"

Stage *MarioState::stage = NULL;

MarioState::MarioState() {}

void MarioState::setStage(Stage *stage) {
    this->stage = stage;
}