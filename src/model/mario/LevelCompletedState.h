#pragma once
#include "MarioState.h"

class LevelCompletedState : public MarioState
{
public:
    const MarioState *update(Mario &mario) const;
    bool getIsLevelCompleted() const;
    static const LevelCompletedState *getInstance();

private:
    LevelCompletedState() = default;
    static const LevelCompletedState instance;
};
