#pragma once
#include "texture/Texture.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

enum class Gear { Forward, Reverse };

class Car {
public:
  //  Control setters
  void setThrottle(float value) { throttle = std::clamp(value, 0.0f, 1.0f); }
  void setBrake(float value) { brake = std::clamp(value, 0.0f, 1.0f); }
  void setSteer(float value) { steer = std::clamp(value, -maxSteer, maxSteer); }

  void setGearForward() { gear = Gear::Forward; }
  void setGearReverse() { gear = Gear::Reverse; }

  //  Main update
  void update(float dt);

  //  Monitoring
  glm::vec2 getPosition() const { return position; }
  float getSpeed() const { return speed; }
  float getHeading() const { return heading; }

  Texture carTexture;

  bool loadTexture(const std::string &path) {
    return carTexture.loadFromFile(path);
  }

private:
  //  Physics
  void updateForces();
  void updateMotion(float dt);

  glm::vec2 position{0.0f, -0.8f};
  float speed = 0.0f;
  float heading = 1.57079632679f;
  // Inputs
  float throttle = 0.0f;
  float brake = 0.0f;
  float steer = 0.0f;

  // Vehicle parameters
  const float mass = 800.0f;
  const float wheelbase = 2.5f;
  const float mu = 1.0f;
  const float dragCoeff = 0.3f;
  const float rollingRes = 30.0f;
  const float maxSteer = 0.8f;

  // Forces
  float F_long = 0.0f;
  float dtStep = 0.016f;

  Gear gear = Gear::Forward;

  // Speed limits
  const float MAX_FORWARD_SPEED = 20.0f;
  const float MAX_REVERSE_SPEED = -6.0f;
};
