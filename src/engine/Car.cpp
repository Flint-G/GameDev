#include "../../include/Car.h"
#include <cmath>
#include <algorithm>


void Car::update(float dt) {
    // Constants
    const float ACCEL = 20.0f;       // acceleration power
    const float MAX_SPEED = 40.0f;   // max forward speed
    const float MAX_REVERSE = -15.0f; // max reverse speed
    const float DRAG = 8.0f;         // slows car down
    const float TURN_RATE = 3.5f;    // steering speed
    const float CAR_HALF_WIDTH  = 0.1f; // half width of car in NDC
    const float CAR_HALF_HEIGHT = 0.05f; // half height of car in NDC

    // 1) Acceleration (forward/backward)
    speed += throttle * ACCEL * dt;

    // 2) Clamp speed to max forward/reverse
    speed = std::clamp(speed, MAX_REVERSE, MAX_SPEED);

    // 3) Apply drag (natural slowdown)
    if (speed > 0.0f) {
        speed = std::max(0.0f, speed - DRAG * dt);
    } else if (speed < 0.0f) {
        speed = std::min(0.0f, speed + DRAG * dt);
    }

    // 4) Steering (only when moving)
    float speedFactor = std::abs(speed) / MAX_SPEED; // 0..1
    heading += steer * TURN_RATE * speedFactor * dt;

    // 5) Movement
    glm::vec2 forward(std::cos(heading), std::sin(heading));
    position += forward * speed * dt;

    // 6) Clamp to screen boundaries
    position.x = std::clamp(position.x, -1.0f + CAR_HALF_WIDTH, 1.0f - CAR_HALF_WIDTH);
    position.y = std::clamp(position.y, -1.0f + CAR_HALF_HEIGHT, 1.0f - CAR_HALF_HEIGHT);
}
