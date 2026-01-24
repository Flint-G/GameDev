#pragma once
#include <glm/glm.hpp>
#include "Collider.h"

class Tree {
public:
    Tree(glm::vec2 pos, float headingVal = 0.0f)
        : heading(headingVal) {
            collider.position = pos;
            collider.radius = 0.2f; 
            collider.isStatic = true; // Trees don't move
            collider.mass = 1000.0f;
            collider.restitution = 0.1f;
        }

    glm::vec2 getPosition() const { return collider.position; }
    void setPosition(const glm::vec2& pos) { collider.position = pos; }
    float getHeading() const { return heading; }

    Collider collider;

private:
    float heading;
};
