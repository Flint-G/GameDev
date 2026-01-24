#pragma once
#include <glm/glm.hpp>
#include "Collider.h"

class Rock {
public:
    Rock(glm::vec2 pos, float headingVal)
        : heading(headingVal) {
            collider.position = pos;
            collider.radius = 0.15f; 
            collider.isStatic = true;
            collider.mass = 500.0f; 
            collider.restitution = 0.2f;
        }

    glm::vec2 getPosition() const { return collider.position; }
    void setPosition(const glm::vec2& pos) { collider.position = pos; }
    float getHeading() const { return heading; }

    Collider collider;

private:
    float heading;
};
