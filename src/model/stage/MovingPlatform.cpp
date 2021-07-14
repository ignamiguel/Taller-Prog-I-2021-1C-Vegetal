#include "MovingPlatform.h"
#include "../../utils/Constants.hpp"

#define MIN_X 17
#define MAX_X 191
#define LEVEL_0_Y 124

#define X_PLAT_INDEX 58
#define Y_PLAT_INDEX 40
#define PLATFORMS_PER_CABLE 3
#define PLATFORM_SPEED 0.25

MovingPlatform::MovingPlatform(const int platform, const int level)
: Platform(0, 0, 0, 0), direction(((level + 1) & 2) - 1) {
    const int limit = ((MAX_X + MIN_X) / 2) + direction * ((MAX_X - MIN_X) / 2);

    start.x = limit - direction * platform * X_PLAT_INDEX;
    start.y = LEVEL_0_Y + (level / 2) * Y_PLAT_INDEX + (level % 2) * ALTO_PLATAFORMA;
    end.x = start.x + ANCHO_PLATAFORMA;
    end.y = start.y;
}

void MovingPlatform::getLimits(float &min, float &max) const {
    min = MIN_X;
    max = MAX_X + ANCHO_PLATAFORMA;
}

float MovingPlatform::getSpeed() const {
    return this->direction * PLATFORM_SPEED;
}

void MovingPlatform::move() {
    start.x += this->direction * PLATFORM_SPEED;
    start.x -= this->direction * (PLATFORMS_PER_CABLE * X_PLAT_INDEX) * ((start.x < MIN_X) || (MAX_X < start.x));
    end.x = start.x + ANCHO_PLATAFORMA;
}

punto_t MovingPlatform::getPos() const {
    return start;
}