#pragma once
#include "texture/Texture.h"
#include "Collider.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

enum class Gear { Forward, Reverse };

class Car {
public:
  void setThrottle(float value) { throttle = std::clamp(value, 0.0f, 1.0f); }
  void setBrake(float value) { brake = std::clamp(value, 0.0f, 1.0f); }
  void setSteer(float value) { steer = std::clamp(value, -maxSteer, maxSteer); }

  void setGearForward() { gear = Gear::Forward; }
  void setGearReverse() { gear = Gear::Reverse; }

  void update(float dt);
  void syncFromCollider(); // Sync physics back from collider
  glm::vec2 getPosition() const { return collider.position; }
  float getSpeed() const { return speed; }
  float getHeading() const { return heading; }

  Texture carTexture;
  Collider collider;

  bool loadTexture(const std::string &path) {
    if(carTexture.loadFromFile(path)) {
        // Initialize collider defaults
        collider.position = {0.0f, -0.8f};
        collider.radius = 0.3f; // Approximation
        collider.isStatic = false;
        collider.mass = mass;
        collider.restitution = 0.4f;
        return true;
    }
    return false;
  }

private:
  void updateForces();
  void updateMotion(float dt);

  // glm::vec2 position{0.0f, -0.8f}; // Removed in favor of collider.position
  float speed = 0.0f;
  float heading = 1.57079632679f;
  float throttle = 0.0f;
  float brake = 0.0f;
  float steer = 0.0f;

  const float mass = 1200.0f; // Heavier car
  const float wheelbase = 2.8f;
  const float mu = 1.2f; // Better grip
  const float dragCoeff = 0.5f; // More air drag
  const float rollingRes = 40.0f;
  const float maxSteer = 0.6f; // Slightly less twitchy

  float F_long = 0.0f;
  float dtStep = 0.016f;

  Gear gear = Gear::Forward;
  const float MAX_FORWARD_SPEED = 28.0f; // Higher top speed
  const float MAX_REVERSE_SPEED = -8.0f;
};
