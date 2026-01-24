#include "../../include/Engine.h"
#include "../../include/Car.h"
#include "../../include/Collision.h"
#include "../../libs/glad/glad.h"
#include "../texture/Texture.h"
#include "../render/Renderer.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

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
  if (!rockTexture.loadFromFile("assets/rock.png")) {
    std::cerr << "Failed to load rock texture!\n";
  }
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  if (!car.loadTexture("assets/car.png")) {
    std::cerr << "Failed to load car texture!" << std::endl;
  }
  if (!lineTexture.loadFromFile("assets/roadStripe.png")) {
      std::cerr << "Failed to load roadStripe texture!\n";
  }
  if (!treeTexture.loadFromFile("assets/tree.png")) {
      std::cerr << "Failed to load tree texture!\n";
  }

  // Initialize objects
  rocks.clear();
  trees.clear();
  lines.clear();

  // Helper lambda for random float
  auto randomFloat = [](float min, float max) {
      return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
  };

  // Spawn objects along the track
  for (int i = 0; i < 50; ++i) {
      float y = i * 2.0f + randomFloat(-0.5f, 0.5f);
      
      // Left Side
      float xLeft = randomFloat(-1.8f, -0.6f); 
      // Right Side
      float xRight = randomFloat(0.6f, 1.8f);

      // Randomly choose side or both for variety
      if (rand() % 2 == 0) {
          trees.emplace_back(glm::vec2(xLeft, y), 0.0f);
      } else {
          trees.emplace_back(glm::vec2(xRight, y), 0.0f);
      }

      // Add rocks occasionally
      if (i % 5 == 0) {
          float rockX = (rand() % 2 == 0) ? xLeft : xRight;
          rocks.emplace_back(glm::vec2(rockX, y + 1.0f), randomFloat(0.0f, 6.0f));
      }
  }

  // Initialize road lines
  for (int i = -10; i < 20; ++i) {
      RoadLine line;
      line.position = glm::vec2(0.0f, i * 0.8f); 
      line.size = glm::vec2(0.02f, 0.3f);         
      lines.push_back(line);
  }

  renderer.init();
  if (!textRenderer.init("assets/Roboto-Regular.ttf")) {
      std::cout << "Warning: Failed to load font!\n";
  }

  // Initialize Client
  if (!client.connect("127.0.0.1")) {
      std::cout << "Failed to connect to server (Is it running?). Playing in offline mode.\n";
  }

  return true;
}

void Engine::handleInput() {
  GLFWwindow *win = window.getHandle();

  // Reset inputs every frame
  car.setThrottle(0.0f);
  car.setBrake(0.0f);
  car.setSteer(0.0f);

  // Forward / Reverse / Brake
  if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
    car.setGearForward();
    car.setThrottle(1.0f);
  } else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
    car.setGearReverse();
    car.setThrottle(1.0f);
  }

  // Space bar as emergency brake
  if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {
    car.setBrake(1.0f);
  }
  // Steering (A / D)
  float steer = 0.0f;
  const float MAX_STEER = 0.35f; // ~20 degrees

  if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
    steer += MAX_STEER;

  if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
    steer -= MAX_STEER;

  car.setSteer(steer);

  // Exit
  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);
}

void Engine::run() {
    float lastTime = glfwGetTime();
    
    // Background scroll factor controls how fast the background moves relative to car
    glm::vec2 bgOffset(0.0f, 0.0f);


    while (!window.shouldClose()) {
        float currentTime = glfwGetTime();
        float deltaTime = std::clamp(currentTime - lastTime, 0.001f, 0.03f);
        lastTime = currentTime;

        if (client.hasSpawned) {
             car.collider.position = client.myStartPosition;
             car.syncFromCollider();
             client.hasSpawned = false; // Only once
        }

        handleInput();
        client.update(deltaTime);
        client.sendPosition(car.getPosition());
        
        car.update(deltaTime); 

        // Collision Checks
        // 1. Car vs Rocks
        for (auto& rock : rocks) {
            if (Collision::checkCollision(car.collider, rock.collider)) {
                Collision::resolveCollision(car.collider, rock.collider);
                car.syncFromCollider();
            }
        }
        // 2. Car vs Trees
        for (auto& tree : trees) {
            if (Collision::checkCollision(car.collider, tree.collider)) {
                Collision::resolveCollision(car.collider, tree.collider);
                car.syncFromCollider();
            }
        }

        // Camera follows the car
        glm::vec2 cameraPos = car.getPosition();
        cameraPos.x = 0.0f; 
        cameraPos.y += 0.5f; 
        
        renderer.setCameraPosition(cameraPos);

        float recycleThreshold = cameraPos.y - 4.5f; 
        float respawnY = cameraPos.y + 2.5f;         

        // Recycle lines
        for (auto& line : lines) {
            if (line.position.y < recycleThreshold) {
                
                line.position.y += 30 * 0.8f; 
            }
        }

        // Recycle rocks
        for (auto& rock : rocks) {
             if (rock.getPosition().y < recycleThreshold) {
                 float newY = respawnY + (rand() % 50) / 10.0f; 
                 float newX = (rand() % 2 == 0) ? -1.5f : 1.5f; 
                 newX += (rand() % 100 / 100.0f) * 0.4f - 0.2f;
                 rock.setPosition(glm::vec2(newX, newY));
             }
        }

        // Recycle trees
        for (auto& tree : trees) {
             if (tree.getPosition().y < recycleThreshold) {
                 float newY = respawnY + (rand() % 50) / 10.0f;
                 float newX = (rand() % 2 == 0) ? -1.5f : 1.5f;
                 newX += (rand() % 100 / 100.0f) * 0.4f - 0.2f;
                 tree.setPosition(glm::vec2(newX, newY));
             }
        }

        renderer.clear();
        renderer.drawBackground(bgTexture); 
        
        for (const auto& line : lines) {
            renderer.drawRoadLine(line, lineTexture);
        }

        for (const auto& rock : rocks) {
            renderer.drawRock(rock, rockTexture);
        }

        for (const auto& tree : trees) {
            renderer.drawTree(tree, treeTexture);
        }

        for (const auto& tree : trees) {
            renderer.drawTree(tree, treeTexture);
        }

        // Draw local car
        renderer.drawCar(car);

        // Draw remote cars
        for (const auto& [id, player] : client.otherPlayers) {
             // For now, assume same car texture and heading up
             renderer.drawCarAt(player.currentPos, 3.14159f / 2.0f, car.carTexture);
             
             // Draw Name
             glm::mat4 proj = glm::ortho(-1.8f, 1.8f, -1.0f, 1.0f); // Match game aspect (roughly)
             textRenderer.setProjection(proj);
             // Offset text slightly above car
             textRenderer.drawText(player.name, player.currentPos.x - 0.1f, player.currentPos.y + 0.2f, 0.001f, glm::vec3(1.0f, 1.0f, 1.0f));
        }                   
        
        window.swap();
        window.poll();
    }
}


void Engine::shutdown() { window.destroy(); }
