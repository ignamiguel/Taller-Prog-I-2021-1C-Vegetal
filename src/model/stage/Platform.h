#pragma once
#include "../../utils/punto.h"

class Platform
{
public:
    Platform(float x1, float y1, float x2, float y2);
    punto_t getRandomPoint(int entityWidth);
    virtual void getLimits(float *min, float *max);
    void getCurrentLimits(float *min, float *max);
    float getY(float x);
    virtual float getSpeed();
    virtual ~Platform() = default;

protected:
    punto_t start;
    punto_t end;
};