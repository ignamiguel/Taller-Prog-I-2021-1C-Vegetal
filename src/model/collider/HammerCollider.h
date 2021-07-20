#pragma once
#include "Collider.h"

const int HAMMER_USES = 3;

class HammerCollider: public Collider {
public:
    HammerCollider();
    void collide(Mario* mario, Entidad* entidad);
    ColliderType getType();
private:
    int uses = HAMMER_USES;
    void decreaseUses(Mario* mario);
    
};