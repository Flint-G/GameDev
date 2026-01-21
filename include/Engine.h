#pragma once

#include "../src/render/Window.h"
#include "../src/render/Renderer.h"
#include "Car.h"

class Engine {
public:
    bool init();
    void run();
    void shutdown();

private:
    void handleInput();

private:
    Window window;
    Renderer renderer;
    Car car;
};
