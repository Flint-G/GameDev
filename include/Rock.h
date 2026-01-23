#pragma once
#include <glm/glm.hpp>

class Rock {
public:
    Rock(glm::vec2 pos, float headingVal)
        : position(pos), heading(headingVal) {}

    glm::vec2 getPosition() const { return position; }
    void setPosition(const glm::vec2& pos) { position = pos; }
    float getHeading() const { return heading; }

    

private:
    glm::vec2 position;
    float heading;
};
