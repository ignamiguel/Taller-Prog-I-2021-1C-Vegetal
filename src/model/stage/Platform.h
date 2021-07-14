#pragma once
#include "../../utils/punto.h"

class Platform
{
public:
    Platform(float x1, float y1, float x2, float y2);
    punto_t getRandomPoint(int entityWidth) const;
    virtual void getLimits(float &min, float &max) const;
    void getCurrentLimits(float &min, float &max) const;
    float getY(float x) const;
    virtual float getSpeed() const;
    virtual ~Platform() = default;

protected:
    punto_t start;
    punto_t end;
};