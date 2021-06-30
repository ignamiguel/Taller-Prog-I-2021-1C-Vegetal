#pragma once
#include "Platform.h"

class MovingPlatform : public Platform
{
public:
    MovingPlatform(int platform, int level);
    void move();
    void getLimits(float *min, float *max);
    float getSpeed();
    punto_t getPos();

private:
    int direction;
};