#include "../../include/Engine.h"
#include "../../include/Car.h"
#include "../../libs/glad/glad.h"
#include "../texture/Texture.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <iostream>

bool Engine::init() {
  if (!window.createFullscreen("Himalayan F1"))
    return false;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return false;
  }
  if (!bgTexture.loadFromFile("assets/background.png")) {
    std::cerr << "Failed to load background texture!\n";
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  if (!car.loadTexture("assets/car.png")) {
    std::cerr << "Failed to load car texture!" << std::endl;
  }

  renderer.init();
  return true;
}

void Engine::handleInput() {
  GLFWwindow *win = window.getHandle();

  // Reset inputs every frame
  car.setThrottle(0.0f);
  car.setBrake(0.0f);
  car.setSteer(0.0f);

  // ======================
  // Forward / Reverse / Brake
  // ======================
  if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
    car.setGearForward();
    car.setThrottle(1.0f);
  } else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
    car.setGearReverse();
    car.setThrottle(1.0f);
  }

  // Optional: Space bar as emergency brake
  if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {
    car.setBrake(1.0f);
  }

  // ======================
  // Steering (A / D)
  // ======================
  float steer = 0.0f;
  const float MAX_STEER = 0.35f; // ~20 degrees

  if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
    steer += MAX_STEER;

  if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
    steer -= MAX_STEER;

  car.setSteer(steer);

  // ======================
  // Exit
  // ======================
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
}

void Engine::run() {
    float lastTime = glfwGetTime();
    
    // Background scroll factor controls how fast the background moves relative to car
    const float scrollFactor = 0.05f;  
    glm::vec2 bgOffset(0.0f, 0.0f); // initial offset

    while (!window.shouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = std::clamp(currentTime - lastTime, 0.001f, 0.03f);
        lastTime = currentTime;

        handleInput();       // update car controls
        car.update(deltaTime); // update car physics

        // Calculate background offset so it loops while car moves
        // Only scale by scrollFactor to slow down background movement
        bgOffset.x = -car.getPosition().x * scrollFactor;
        bgOffset.y = -car.getPosition().y * scrollFactor;

        renderer.clear();                        // clear screen
        renderer.drawBackground(bgTexture); // draw scrolling background
        renderer.drawCar(car);                   // draw car at center

        window.swap();
        window.poll();
    }
}


void Engine::shutdown() { window.destroy(); }
