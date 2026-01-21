#include "Renderer.h"
#include "../../libs/glad/glad.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Vertex & fragment shaders
const std::string vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;

uniform mat4 uModel;
uniform mat4 uProj;

void main() {
    gl_Position = uProj * uModel * vec4(aPos, 0.0, 1.0);
}
)";

const std::string fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

Renderer::~Renderer() {
    if (shader) delete shader;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Renderer::init() {
    shader = new Shader(vertexShaderSrc, fragmentShaderSrc);
    if (!shader || shader->getID() == 0) {
        std::cerr << "Failed to create shader!\n";
        return;
    }

    float vertices[] = {
        -0.1f, -0.05f,
         0.1f, -0.05f,
         0.1f,  0.05f,

         0.1f,  0.05f,
        -0.1f,  0.05f,
        -0.1f, -0.05f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::drawCar(const Car& car) {
    if (!shader || shader->getID() == 0) return;

    glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();

    // Build transform matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(car.getPosition(), 0.0f));
    model = glm::rotate(model, car.getHeading(), glm::vec3(0.0f, 0.0f, 1.0f));

    // Projection (so it fits in view)
    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

    shader->setMat4("uModel", model);
    shader->setMat4("uProj", proj);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
