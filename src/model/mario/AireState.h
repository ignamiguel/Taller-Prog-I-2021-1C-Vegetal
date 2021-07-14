#pragma once
#include "MarioState.h"

class AireState : public MarioState
{
public:
    const MarioState *update(Mario &mario) const;
    static const AireState *getInstance();

private:
    AireState() = default;
    static const AireState instance;
};
