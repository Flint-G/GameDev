#pragma once

#include "../src/render/Window.h"
#include "../src/render/Renderer.h"
#include "Car.h"
#include "Rock.h"
#include "RoadLine.h"
#include "Tree.h"
#include <vector>

#include "../src/texture/Texture.h"

class Engine {
public:
    bool init();
    void run();
    void shutdown();

private:
    void handleInput();

private:
    float scrollFactor = 0.5f;
    glm::vec2 bgOffset {0.0f, 0.0f};
    Window window;
    Renderer renderer;
    Car car;
    Texture bgTexture;
    Texture rockTexture;
    Texture lineTexture;
    Texture treeTexture;
    std::vector<Rock> rocks;
    std::vector<RoadLine> lines;
    std::vector<Tree> trees;
};


