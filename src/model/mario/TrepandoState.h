#pragma once
#include "MarioState.h"

class TrepandoState : public MarioState
{
public:
    const MarioState *update(Mario &mario) const;
    static const TrepandoState *getInstance();

private:
    TrepandoState() = default;
    static const TrepandoState instance;
};
