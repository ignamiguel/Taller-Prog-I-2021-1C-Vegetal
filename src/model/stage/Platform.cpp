#include <stdlib.h>
#include <cmath>
#include "Platform.h"

Platform::Platform(float x1, float y1, float x2, float y2) {
    this->start.x = x1;
    this->start.y = y1;

    this->end.x = x2;
    this->end.y = y2;
}

punto_t Platform::getRandomPoint(int entityWidth) {
    punto_t pos;

    float t = rand() / (float)RAND_MAX;
    pos.x = start.x + t * ((end.x - entityWidth) - start.x);
    pos.y = start.y + t * (end.y - start.y);

    return pos;
}

void Platform::getLimits(float *min, float *max) {
    getCurrentLimits(min, max);
}

void Platform::getCurrentLimits(float *min, float *max) {
    *min = start.x;
    *max = end.x;
}

float Platform::getY(float x) {
    return start.y + (x - start.x) / (end.x - start.x) * (end.y - start.y);
}

float Platform::getSpeed() {
    return 0;
}

float Platform::getXMovement() {
    float distanceX = (end.x - start.x) / (end.x - start.x + fabs(end.y - start.y));
    // decide hacia que lado ir segun la inclinacion de la plataforma
    // si es horizontal no se mueve
    if ((end.y - start.y) < 0) return (-1) * distanceX;
    return distanceX;
}

float Platform::getYMovement() {
    return fabs(end.y - start.y) / (end.x - start.x + fabs(end.y - start.y));
}