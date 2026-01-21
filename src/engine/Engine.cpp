#include "../../libs/glad/glad.h"
#include "../../include/Engine.h"
#include "../../include/Car.h"

#include <iostream>

bool Engine::init() {
  if (!window.createFullscreen("Himalayan F1")) return false;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    renderer.init();
    return true;
}
void Engine::handleInput() {
    GLFWwindow* win = window.getHandle();

    // reset input state
    car.setThrottle(0.0f);
    car.setSteer(0.0f);

    // throttle
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        car.setThrottle(1.0f);
    else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        car.setThrottle(-1.0f);

    // steering
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        car.setSteer(1.0f);
    else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        car.setSteer(-1.0f);

    // exit
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);
}


void Engine::run() {
    float lastTime = glfwGetTime();

    while (!window.shouldClose()) {
        const float currentTime = glfwGetTime();
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        handleInput();
        car.update(deltaTime);
        renderer.drawCar(car);
        window.swap();
        window.poll();
    }
}

void Engine::shutdown() {
    window.destroy();
}
