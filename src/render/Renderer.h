#pragma once
#include "../../shaders/Shader.h"
#include "../../include/Car.h"

class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    void init();
    void drawCar(const Car& car);
private:
    Shader* shader = nullptr;
    unsigned int VAO = 0, VBO = 0;
};
