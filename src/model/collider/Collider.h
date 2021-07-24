#pragma once
#include "../Entidad.h"

class Mario;

enum ColliderType
{
    NORMAL,
    TEST_MODE,
    HAMMER
};

class Collider {
public:
    virtual ~Collider() = default;
    virtual void collide(Mario* mario, Entidad* entidad) = 0;
    virtual ColliderType getType() = 0;
private:
};