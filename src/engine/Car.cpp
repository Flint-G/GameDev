#include "Car.h"
#include <algorithm>
#include <cmath>

constexpr float WORLD_MIN_X = -0.95f;
constexpr float WORLD_MAX_X =  0.95f;
constexpr float WORLD_MIN_Y = -0.95f;
constexpr float WORLD_MAX_Y =  0.95f;

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

    // Update position
    glm::vec2 forward(std::cos(heading), std::sin(heading));
    position += forward * speed * dt;

    // Clamp to world boundaries
    position.x = std::clamp(position.x, WORLD_MIN_X, WORLD_MAX_X);
    position.y = std::clamp(position.y, WORLD_MIN_Y, WORLD_MAX_Y);

    // Stop car if hitting boundaries
    if (position.x == WORLD_MIN_X || position.x == WORLD_MAX_X ||
        position.y == WORLD_MIN_Y || position.y == WORLD_MAX_Y) {
        speed = 0.0f;
    }
}
