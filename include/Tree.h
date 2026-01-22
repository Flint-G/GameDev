#pragma once
#include <glm/glm.hpp>

class Tree {
public:
    Tree(glm::vec2 pos, float headingVal = 0.0f)
        : position(pos), heading(headingVal) {}

    glm::vec2 getPosition() const { return position; }
    void setPosition(const glm::vec2& pos) { position = pos; }
    float getHeading() const { return heading; }


private:
    glm::vec2 position;
    float heading;
};
