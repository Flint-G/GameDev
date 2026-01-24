#include "../../include/Collision.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

bool Collision::checkCollision(const Collider& a, const Collider& b){
    float dist2 = glm::length2(a.position - b.position);
    float radiusSum = a.radius + b.radius;
    return dist2 <= radiusSum * radiusSum;
}

void Collision::resolveCollision(Collider& a, Collider& b){
    glm::vec2 delta = a.position - b.position;
    float dist = glm::length(delta);
    float radiusSum = a.radius + b.radius;

    if(dist >= radiusSum || dist == 0.0f) return; // No overlap or exact overlap (avoid div by 0)

    // 1. Positional Correction (prevent sinking)
    glm::vec2 normal = delta / dist;
    float penetration = radiusSum - dist;
    
    float totalMass = (a.isStatic ? 0 : a.mass) + (b.isStatic ? 0 : b.mass);
    if(totalMass == 0) return; // Both static

    float aInvMass = a.isStatic ? 0 : 1.0f / a.mass;
    float bInvMass = b.isStatic ? 0 : 1.0f / b.mass;
    float invMassSum = aInvMass + bInvMass;

    glm::vec2 correction = normal * (penetration / invMassSum);
    if(!a.isStatic) a.position += correction * aInvMass;
    if(!b.isStatic) b.position -= correction * bInvMass;

    // 2. Impulse Resolution (elasticity)
    glm::vec2 relativeVel = a.velocity - b.velocity;
    float velAlongNormal = glm::dot(relativeVel, normal);

    // Do not resolve if velocities are separating
    if (velAlongNormal > 0) return;

    float e = std::min(a.restitution, b.restitution);
    float j = -(1 + e) * velAlongNormal;
    j /= invMassSum;

    glm::vec2 impulse = j * normal;
    if(!a.isStatic) a.velocity += impulse * aInvMass;
    if(!b.isStatic) b.velocity -= impulse * bInvMass;
}