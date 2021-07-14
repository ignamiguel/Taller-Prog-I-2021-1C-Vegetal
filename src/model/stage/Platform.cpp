#include <stdlib.h>
#include "Platform.h"

Platform::Platform(const float x1, const float y1, const float x2, const float y2)
: start{x1, y1}, end{x2, y2} {}

punto_t Platform::getRandomPoint(const int entityWidth) const {
    const float t = rand() / (float)RAND_MAX;
    const float x = start.x + t * ((end.x - entityWidth) - start.x);
    const float y = start.y + t * (end.y - start.y);

    return punto_t{x, y};
}

void Platform::getLimits(float &min, float &max) const {
    getCurrentLimits(min, max);
}

void Platform::getCurrentLimits(float &min, float &max) const {
    min = start.x;
    max = end.x;
}

float Platform::getY(const float x) const {
    return start.y + (x - start.x) / (end.x - start.x) * (end.y - start.y);
}

float Platform::getSpeed() const {
    return 0;
}