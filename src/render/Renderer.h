#pragma once
#include "../../shaders/Shader.h"
#include "../../include/Car.h"
#include "../../src/texture/Texture.h"

class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    void init();
    void clear(); // clear screen
    void drawCar(const Car& car);
    void drawBackground(const Texture& bg); // single argument

private:
    Shader* shader = nullptr;
    unsigned int VAO = 0, VBO = 0, EBO = 0;
};
