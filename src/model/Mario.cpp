#include "Mario.hpp"
#include "../utils/Constants.hpp"
#include "mario/SueloState.h"
#include "mario/MuriendoState.h"
#include "collider/Collider.h"
#include "collider/TestCollider.h"
#include "collider/NormalCollider.h"

#define MARIO_X_DIF 3.f
#define MARIO_Y_DIF 2.f

Mario::Mario() : Entidad(0, 0), state(SueloState::getInstance()) {}

Mario::Mario(const Mario &other) : Entidad(other.pos.x, other.pos.y), 
audioObserver{other.audioObserver}, lives{other.lives}, 
controls{other.controls}, contador{other.contador}, estado{other.estado}, velX{other.velX}, velY{other.velY}, 
climbMin{other.climbMin}, climbMax{other.climbMax}, collider{new NormalCollider()}, direccion{other.direccion},
state{other.state}, score{other.score} {}

void Mario::reset() {
    this->state = this->state->reset(*this);
    if(collider->getType() != TEST_MODE) {
        setCollider(NORMAL);
    }
}

void Mario::setStageAndReset(Stage *stage) {
    MarioState::setStage(stage);
    this->reset();
}

void Mario::mover() {
    this->state = this->state->update(*this);    
    controls.space = 0;
}

void Mario::die() {
    if (contador != 0) return;
    ++contador;
    --lives;
    this->audioObserver.update(DEATH);
    this->state = MuriendoState::getInstance();
}

void Mario::collide(Entidad* entidad) {
    this->collider->collide(this, entidad);
}

void Mario::collide(Hammer* hammer) {
    if(this->collider->getType() != TEST_MODE) {
        delete(this->collider);
        this->collider = new HammerCollider();
        hammer->consume();
        this->audioObserver.update(ITEM);
    }
}


estadoMario_t Mario::getEstado() const {
    return {pos, estado, (collider->getType() == HAMMER), isEnabled, this->audioObserver.getState()};
}

dimensiones_t Mario::dimensions() const {
    return {pos.x + MARIO_X_DIF, pos.y + MARIO_Y_DIF, pos.x + (ANCHO_MARIO - MARIO_X_DIF), pos.y + (ALTO_MARIO - MARIO_Y_DIF)};
}

unsigned char Mario::getScore() const {
    return this->score;
}

void Mario::addPoints(unsigned char points) {
    this->score += points;
}

bool Mario::getIsLevelCompletedOrDisabled() const {
    return (this->state->getIsLevelCompleted() || !this->isEnabled);
}

void Mario::setCollider(ColliderType colliderType) { //para no tener que poner delete cada vez que se quiera cambiar el collider
    delete (this->collider);
    switch(colliderType) {
        case NORMAL: 
            this->collider = new NormalCollider();
            break;
        case TEST_MODE:
            this->collider = new TestCollider();
            break;
        case HAMMER:
            this->collider = new HammerCollider();
            break;
    }
}

void Mario::toggleTestMode() {
    if(this->collider->getType() != TEST_MODE) {
        setCollider(TEST_MODE);
        printf("Modo test: ON\n");
    } else {
        setCollider(NORMAL);
        printf("Modo test: OFF\n");
    }
}

bool Mario::getIsGameOver() const {
    return this->state->getIsGameOver();
}

Mario::~Mario() {
    delete collider;
}
