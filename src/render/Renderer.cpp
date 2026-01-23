#include "Renderer.h"
#include "../../libs/glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Renderer::init() {
    // Vertex shader
    const std::string vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoord;

        uniform mat4 uModel;
        uniform mat4 uProj;
        uniform mat4 uView; 

        out vec2 TexCoord;
        void main() {
            gl_Position = uProj * uView * uModel * vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";



    // Fragment shader
    const std::string fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoord;
        uniform sampler2D uTexture;
        uniform vec2 uTexOffset; 

        void main() {
            FragColor = texture(uTexture, TexCoord + uTexOffset);
        }
    )";


    shader = new Shader(vertexShaderSrc, fragmentShaderSrc);

    // Full-screen quad (covers NDC space [-1,1])
    float vertices[] = {
        // x, y, u, v
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear() {
    glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawBackground(const Texture& bg) {
    if (!shader || shader->getID() == 0) return;

    shader->use();
    bg.bind();

    glm::mat4 view = glm::mat4(1.0f); 
    shader->setMat4("uView", view);
    
    shader->setVec2("uTexOffset", glm::vec2(0.0f, cameraPos.y * 0.05f)); 

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 proj  = glm::mat4(1.0f);
    shader->setMat4("uModel", model);
    shader->setMat4("uProj", proj);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    bg.unbind();
}

void Renderer::drawCar(const Car& car) {
    if (!shader || shader->getID() == 0) return;

    shader->use();
    shader->setVec2("uTexOffset", glm::vec2(0.0f)); 

    car.carTexture.bind();

    float texWidth  = (float)car.carTexture.getWidth();
    float texHeight = (float)car.carTexture.getHeight();
    float aspect    = texWidth / texHeight;

    float desiredHeight = 0.2f;
    float desiredWidth  = desiredHeight * aspect;

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));
    shader->setMat4("uView", view);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(car.getPosition(), 0.0f));

    model = glm::rotate(model, car.getHeading() - 1.57079632679f, glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(desiredWidth, desiredHeight, 1.0f));

    glm::mat4 proj = glm::ortho(-1.f, 1.f, -1.f, 1.f);
    shader->setMat4("uModel", model);
    shader->setMat4("uProj", proj);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    car.carTexture.unbind();
}


void Renderer::drawRock(const Rock& rock, const Texture& texture){
    if (!shader || shader->getID() == 0) return;

    shader->use();
    texture.bind();

    float texWidth  = (float)texture.getWidth();
    float texHeight = (float)texture.getHeight();
    float aspect    = texWidth / texHeight;

    float desiredHeight = 0.2f;
    float desiredWidth  = desiredHeight * aspect;

    // View matrix
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));
    shader->setMat4("uView", view);
    shader->setVec2("uTexOffset", glm::vec2(0.0f));

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(rock.getPosition(), 0.0f));

    model = glm::rotate(model, rock.getHeading() - 1.57079632679f, glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(desiredWidth, desiredHeight, 1.0f));

    glm::mat4 proj = glm::ortho(-1.f, 1.f, -1.f, 1.f);
    shader->setMat4("uModel", model);
    shader->setMat4("uProj", proj);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    texture.unbind();
}

void Renderer::drawTree(const Tree& tree, const Texture& texture) {
    if (!shader || shader->getID() == 0) return;

    shader->use();
    texture.bind();

    float texWidth  = (float)texture.getWidth();
    float texHeight = (float)texture.getHeight();
    float aspect    = texWidth / texHeight;

    float desiredHeight = 0.4f; 
    float desiredWidth  = desiredHeight * aspect;

    // View matrix
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));
    shader->setMat4("uView", view);
    shader->setVec2("uTexOffset", glm::vec2(0.0f));
    
    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(tree.getPosition(), 0.0f));
   
    model = glm::rotate(model, tree.getHeading() - 1.57079632679f, glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(desiredWidth, desiredHeight, 1.0f));

    glm::mat4 proj = glm::ortho(-1.f, 1.f, -1.f, 1.f);
    shader->setMat4("uModel", model);
    shader->setMat4("uProj", proj);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    texture.unbind();
}



void Renderer::drawRoadLine(const RoadLine& line, const Texture& texture) {
    if (!shader || shader->getID() == 0) return;

    shader->use();
    texture.bind();

    // View matrix
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraPos.x, -cameraPos.y, 0.0f));
    shader->setMat4("uView", view);
    shader->setVec2("uTexOffset", glm::vec2(0.0f));

    glm::mat4 model(1.0f);

    model = glm::translate(model, glm::vec3(line.position, 0.0f));
    // No rotation for lines usually, or simple scaling
    model = glm::scale(model, glm::vec3(line.size.x, line.size.y, 1.0f));

    glm::mat4 proj = glm::ortho(-1.f, 1.f, -1.f, 1.f);
    shader->setMat4("uModel", model);
    shader->setMat4("uProj", proj);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    texture.unbind();
}



Renderer::~Renderer() {
    if(shader) delete shader;
    if(VAO) glDeleteVertexArrays(1, &VAO);
    if(VBO) glDeleteBuffers(1, &VBO);
    if(EBO) glDeleteBuffers(1, &EBO);
}
