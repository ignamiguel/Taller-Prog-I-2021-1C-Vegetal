#pragma once
#include "MarioState.h"

class SueloState : public MarioState
{
public:
    const MarioState *update(Mario &mario) const;
    static const SueloState *getInstance();

private:
    SueloState() = default;
    static const SueloState instance;
};
