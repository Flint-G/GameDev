#pragma once
#include "Collider.h"

class Collision{
    public:
    static bool checkCollision(const Collider& a, const Collider& b);
    static void resolveCollision(Collider& a, Collider& b);
};