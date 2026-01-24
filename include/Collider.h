#pragma once
#include <glm/glm.hpp>

struct Collider{
    glm::vec2 position;
    float radius;
    bool isStatic = true;
    glm::vec2 velocity = {0.0f, 0.0f};
    float mass = 1.0f;
    float restitution = 0.5f; // Bounciness
};