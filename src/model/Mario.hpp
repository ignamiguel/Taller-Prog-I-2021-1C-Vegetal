#pragma once
#include "Entidad.h"
#include "mario/MarioState.h"
#include "../controller/AudioObserver.h"
#include "../utils/marioStructs.h"
#include "collider/TestCollider.h"
#include "collider/NormalCollider.h"
#include "collider/HammerCollider.h"
#include "../utils/Constants.hpp"
#include "Hammer.h"

class MarioState;
class Collider;

class Mario : public Entidad
{
public:
    Mario();
    Mario(const Mario &other);
    ~Mario();
    void reset();
    void setStageAndReset(Stage *stage);
    void mover();
    void die();
    void disable() { isEnabled = false; controls = {}; }
    void enable() { isEnabled = true; }
    estadoMario_t getEstado() const;
    dimensiones_t dimensions() const;
    unsigned char getScore() const;
    void addPoints(unsigned char points);
    bool getIsLevelCompletedOrDisabled() const;
    bool getIsGameOver() const;
    void collide(Entidad* entidad);
    void collide(Hammer* hammer);
    void toggleTestMode();
    void setCollider(ColliderType colliderType);

    AudioObserver audioObserver{};
    char lives{MARIO_LIVES};
    controls_t controls{};
    unsigned char contador{0};
    char estado{REPOSO};
    float velX{0.f};
    float velY{0.f};
    float climbMin{256};
    float climbMax{0};
    Collider* collider = new NormalCollider();
    Direccion direccion = DERECHA;
private:
    const MarioState *state;
    unsigned char score{0};
};