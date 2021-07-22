#pragma once
#include "Entidad.h"
#include "mario/MarioState.h"
#include "../controller/AudioObserver.h"
#include "../utils/marioStructs.h"
#include "../utils/Constants.hpp"

class MarioState;

class Mario : public Entidad
{
public:
    Mario();
    void reset();
    void setStageAndReset(Stage *stage);
    void mover();
    void disable() { isEnabled = false; controls = {0, 0, 0, 0, 0}; }
    void enable() { isEnabled = true; }
    estadoMario_t getEstado() const;
    void die();
    dimensiones_t dimensions() const;
    unsigned char getScore();
    void addPoints(unsigned char points);
    bool getIsLevelCompletedOrDisabled();
    bool getIsGameOver();

    AudioObserver audioObserver{};
    char lives{MARIO_LIVES};
    controls_t controls{0, 0, 0, 0, 0};
    unsigned char contador{0};
    char estado{REPOSO};
    float velX{0.f};
    float velY{0.f};
    float climbMin{256};
    float climbMax{0};

private:
    const MarioState *state;
    bool isEnabled{true};
    unsigned char score{0};
};