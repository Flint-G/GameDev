#pragma once

#include "../../libs/glad/glad.h"
#include <string>
#include <map>
#include <glm/glm.hpp>

class TextRenderer {
public:
    TextRenderer() = default;
    ~TextRenderer();

    bool init(const std::string& fontPath);
    void drawText(const std::string& text, float x, float y, float scale, const glm::vec3& color);
    void setProjection(const glm::mat4& proj);

private:
    unsigned int VAO = 0, VBO = 0;
    unsigned int textureID = 0;
    unsigned int shaderProgram = 0;
    
    // stb_truetype baked char data
    struct Character {
        unsigned short x0, y0, x1, y1; // coordinates in texture
        float xoff, yoff, xadvance;
    };
    Character cdata[96]; // ASCII 32..126
};
