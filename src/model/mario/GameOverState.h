#pragma once
#include "MarioState.h"

class GameOverState : public MarioState
{
public:
    const MarioState *update(Mario &mario) const;
    const MarioState *reset(Mario &) const;
    bool getIsLevelCompleted() const;
    bool getIsGameOver() const;
    static const GameOverState *getInstance();

private:
    GameOverState() = default;
    static const GameOverState instance;
};
