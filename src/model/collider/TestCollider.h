#pragma once
#include "Collider.h"

class TestCollider: public Collider {
public:
    void collide(Mario* mario, Entidad* entidad);
    ColliderType getType();
};