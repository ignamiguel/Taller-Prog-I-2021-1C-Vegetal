#pragma once
#include "../../utils/punto.h"

class Ladder
{
public:
    Ladder(float x, float bottom, float top);
    float getX();
    float getBottom();
    float getTop();

private:
    float x;
    float bottom;
    float top;
};