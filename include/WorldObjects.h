#pragma once
#include "Collider.h"
#include <vector>

class WorldObjects{
    public:
    std::vector<Collider> objects;
    void addTree(const glm::vec2& pos , float radius){
        objects.push_back({pos, radius, true});
    };
    void addRock(const glm::vec2& pos , float radius){
        objects.push_back({pos, radius, true});
    };
};