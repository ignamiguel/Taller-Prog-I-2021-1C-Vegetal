#pragma once
#include "MarioState.h"

class MuriendoState : public MarioState
{
public:
    const MarioState *update(Mario &mario) const;
    static const MuriendoState *getInstance();

private:
    MuriendoState() = default;
    static const MuriendoState instance;
};
