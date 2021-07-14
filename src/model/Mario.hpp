#pragma once
#include "Entidad.h"
#include "mario/MarioState.h"
#include "../controller/AudioObserver.h"
#include "../utils/marioStructs.h"

class MarioState;

class Mario : public Entidad
{
public:
    Mario();
    void reset();
    void setStage(const Stage *stage) const;
    void mover();
    void disable() { isEnabled = false; controls = {0, 0, 0, 0, 0}; }
    void enable() { isEnabled = true; }
    estadoMario_t getEstado() const;
    void die();

    AudioObserver audioObserver{};
    char lives{3};
    unsigned char score{0};
    controls_t controls{0, 0, 0, 0, 0};
    unsigned char contador{0};
    char estado{REPOSO};
    float velX{0.f};
    float velY{0.f};
    float climbMin{256};
    float climbMax{0};

    dimensiones_t dimensions();

private:
    const MarioState *state;
    bool isEnabled{true};
};