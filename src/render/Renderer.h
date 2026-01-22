#pragma once
#include "../../shaders/Shader.h"
#include "../../include/Car.h"
#include "../../include/Rock.h"
#include "../../include/RoadLine.h"
#include "../../include/Tree.h"


class Renderer {
public:
    Renderer() = default;
    ~Renderer();

    void init();
    void setCameraPosition(const glm::vec2& pos) { cameraPos = pos; }
    void clear(); // clear screen

    void drawCar(const Car& car);
    void drawBackground(const Texture& bg); // single argument
    void drawRock(const Rock& rock, const Texture& texture);
    void drawTree(const Tree& tree, const Texture& texture);
    void drawRoadLine(const RoadLine& line, const Texture& texture);




private:
    glm::vec2 cameraPos{0.0f, 0.0f};
    Shader* shader = nullptr;

    unsigned int VAO = 0, VBO = 0, EBO = 0;
};
