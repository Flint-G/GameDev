#include "Car.h"
#include <algorithm>
#include <cmath>

constexpr float WORLD_MIN_X = -0.95f;
constexpr float WORLD_MAX_X =  0.95f;


void Car::update(float dt) {
    dtStep = dt;

    updateForces();
    updateMotion(dt);
}

void Car::updateForces() {
    // Engine force depends on gear
    float engineSign = (gear == Gear::Forward) ? 1.0f : -1.0f;
    float F_engine = engineSign * throttle * mu * mass * 9.81f;

    // Brake force
    float F_brake = brake * mu * mass * 9.81f * 1.5f;

    // Drag & rolling resistance
    float F_drag = -dragCoeff * speed * std::abs(speed);
    float F_rr = -rollingRes * speed;

    // Net force
    F_long = F_engine + F_drag + F_rr;

    // Apply brake opposite to motion
    if (std::abs(speed) > 0.01f) {
        F_long -= std::copysign(F_brake, speed);
    }
}

void Car::updateMotion(float dt) {
    // Update speed
    float acceleration = F_long / mass;
    speed += acceleration * dt;

    // Clamp speed to limits
    speed = std::clamp(speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);

    // Steering
    if (std::abs(steer) > 0.001f && std::abs(speed) > 0.01f) {
        float turnRadius = wheelbase / std::tan(steer);
        float angularVelocity = speed / turnRadius;
        heading += angularVelocity * dt;
    }

    // Update velocity vector for collision
    glm::vec2 forward(std::cos(heading), std::sin(heading));
    collider.velocity = forward * speed;

    // Update position
    collider.position += collider.velocity * dt;

    // Clamp to world boundaries (X only, allow infinite Y)
    collider.position.x = std::clamp(collider.position.x, WORLD_MIN_X, WORLD_MAX_X);


    // Stop car if hitting boundaries (X only)
    if (collider.position.x == WORLD_MIN_X || collider.position.x == WORLD_MAX_X) {
        speed = 0.0f;
        collider.velocity = {0.0f, 0.0f};
    }

}

void Car::syncFromCollider() {
    // If collision resolution changed velocity, update 'speed'
    // Project velocity onto heading to get new speed (simplified)
    glm::vec2 forward(std::cos(heading), std::sin(heading));
    float newSpeed = glm::dot(collider.velocity, forward);
    
    // Optional: If significant lateral impulse, maybe adjust heading? 
    // For now, just damp speed and stick to track heading mostly
    
    speed = newSpeed;
}
