#include "NormalCollider.h"
#include "../Mario.hpp"

void NormalCollider::collide(Mario* mario, Entidad* ) {
    mario->die();
}

ColliderType NormalCollider::getType() {
    return NORMAL;
}