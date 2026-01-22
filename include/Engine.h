#pragma once

#include "../src/render/Window.h"
#include "../src/render/Renderer.h"
#include "Car.h"
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
};
