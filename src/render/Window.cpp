#include "Window.h"
#include <iostream>

bool Window::createFullscreen(const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Get primary monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) {
        std::cerr << "Failed to get primary monitor\n";
        return false;
    }

    // Get monitor video mode (resolution, refresh rate)
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Create fullscreen window
    window = glfwCreateWindow(
        mode->width,
        mode->height,
        title,
        monitor,   // passing monitor = fullscreen
        nullptr
    );

    if (!window) {
        std::cerr << "Failed to create fullscreen window\n";
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync
    return true;
}

void Window::poll() { glfwPollEvents(); }
bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}
void Window::swap() { glfwSwapBuffers(window); }

void Window::destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
