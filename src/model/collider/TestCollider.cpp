#include "TestCollider.h"
#include "../Mario.hpp"

void TestCollider::collide(Mario* , Entidad* ) { }

ColliderType TestCollider::getType() {
    return TEST_MODE;
}