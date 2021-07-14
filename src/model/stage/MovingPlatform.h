#pragma once
#include "Platform.h"

class MovingPlatform : public Platform
{
public:
    MovingPlatform(int platform, int level);
    void move();
    void getLimits(float &min, float &max) const;
    float getSpeed() const;
    punto_t getPos() const;

private:
    const int direction;
};