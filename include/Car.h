#pragma once
#include <glm/glm.hpp>

class Car {
public:
    void setThrottle(float value) { throttle = value; }
    void setSteer(float value)    { steer = value; }

    void update(float dt);

    // monitoring
    glm::vec2 getPosition() const { return position; }
    float getSpeed() const { return speed; }
    float getHeading() const { return heading; }

private:
    glm::vec2 position {0.0f, 0.0f};

    float throttle = 0.0f; 
    float steer = 0.0f;    

    float speed = 0.0f;
    float heading = 0.0f;  
};
