#pragma once
#include "../../libs/glad/glad.h"
#include <GLFW/glfw3.h>

class Window {
public:
    bool createFullscreen(const char* title);
    void poll();
    bool shouldClose() const;
    void swap();
    void destroy();

    GLFWwindow* getHandle() { return window; }

private:
    GLFWwindow* window = nullptr;
};
