#define STB_TRUETYPE_IMPLEMENTATION
#include "../../resources/vendors/stb_truetype/stb_truetype.h"
#include "TextRenderer.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TextRenderer::~TextRenderer() {
    if(VAO) glDeleteVertexArrays(1, &VAO);
    if(VBO) glDeleteBuffers(1, &VBO);
    if(textureID) glDeleteTextures(1, &textureID);
    if(shaderProgram) glDeleteProgram(shaderProgram);
}

bool TextRenderer::init(const std::string& fontPath) {
    // 1. Load Font File
    std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open font file: " << fontPath << "\n";
        return false;
    }
    std::streampos size = file.tellg();
    std::vector<unsigned char> fontBuffer(size);
    file.seekg(0, std::ios::beg);
    file.read((char*)fontBuffer.data(), size);
    file.close();

    // 2. Prepare Bitmap
    const int BITMAP_W = 512;
    const int BITMAP_H = 512;
    std::vector<unsigned char> tempBitmap(BITMAP_W * BITMAP_H);

    stbtt_bakedchar bakedChars[96];
    stbtt_BakeFontBitmap(fontBuffer.data(), 0, 32.0f, tempBitmap.data(), BITMAP_W, BITMAP_H, 32, 96, bakedChars);

    // Convert packed coords to our struct
    for(int i = 0; i < 96; ++i) {
        cdata[i].x0 = bakedChars[i].x0;
        cdata[i].y0 = bakedChars[i].y0;
        cdata[i].x1 = bakedChars[i].x1;
        cdata[i].y1 = bakedChars[i].y1;
        cdata[i].xoff = bakedChars[i].xoff;
        cdata[i].yoff = bakedChars[i].yoff;
        cdata[i].xadvance = bakedChars[i].xadvance;
    }

    // 3. Create Texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, BITMAP_W, BITMAP_H, 0, GL_RED, GL_UNSIGNED_BYTE, tempBitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. Create Shader
    const char* vShaderCode = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
        out vec2 TexCoords;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
    )";
    const char* fShaderCode = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;
        uniform sampler2D text;
        uniform vec3 textColor;
        void main() {
            float alpha = texture(text, TexCoords).r;
            color = vec4(textColor, alpha); // Text color with alpha from RED channel
        }
    )";
    
    // Compile Shaders
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vShaderCode, NULL);
    glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fShaderCode, NULL);
    glCompileShader(fs);
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vs);
    glDeleteShader(fs);

    // 5. Setup VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void TextRenderer::setProjection(const glm::mat4& proj) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
}

void TextRenderer::drawText(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // We assume incoming coordinates are in World Space (mostly)
    // But typically text renderers work in Screen Space or World Space.
    // If we use the same Ortho projection as the game (-1 to 1), then scale needs to be small (e.g. 0.001) or pixels.
    // Let's assume the projection matrix handles it.

    float startX = x;

    for (const char& c : text) {
        if (c < 32 || c > 126) continue;
        
        // Character& ch = cdata[c - 32];
        // Cannot use reference if we copy from bake?
        // Actually we just use raw bake calculations or our struct
        
        int idx = c - 32;
        float qx0 = cdata[idx].x0;
        float qy0 = cdata[idx].y0;
        float qx1 = cdata[idx].x1;
        float qy1 = cdata[idx].y1;
        
        float xoff = cdata[idx].xoff;
        float yoff = cdata[idx].yoff;
        float xarg = x + (xoff * scale);
        // y is usually baseline. stbtruetype y is down?
        // in OpenGL Y is up. 
        // We might need to flip Y or adjust. 
        // Standard stb_truetype is Y down.
        // Let's try inverting yoff
        float yarg = y - (yoff * scale); // Minus?

        float w = (qx1 - qx0) * (512.0f/512.0f); // just logic check
        float h = (qy1 - qy0);

        // Normalize texture coords
        float s0 = qx0 / 512.0f;
        float t0 = qy0 / 512.0f;
        float s1 = qx1 / 512.0f;
        float t1 = qy1 / 512.0f;

        // Quad in world space
        // Assuming height/width is in pixels, we scale them down to world units
        // 32px font -> scale 0.01 -> 0.32 world units
        float w_world = (xoff + (qx1-qx0)) * scale - (xoff * scale); // actual char width
        w_world = (qx1 - qx0) * scale;
        float h_world = (qy1 - qy0) * scale;

        // Position
        // y is top? or baseline?
        // yarg is top-left
        GLfloat xpos = x + cdata[idx].xoff * scale;
        GLfloat ypos = y - (cdata[idx].yoff + (qy1 - qy0)) * scale; // Adjust for bottom-left origin of quad?
        
        // This is tricky without trial/error. 
        // Simpler approach: 
        // xpos, ypos, w, h
        xpos = x + cdata[idx].xoff * scale;
        ypos = y - (cdata[idx].yoff * scale); // if yoff is positive downwards
        // But we want to render it upright. 
        
        // Let's try standard quad
        float wVal = (qx1 - qx0) * scale;
        float hVal = (qy1 - qy0) * scale;
        
        xpos = x + cdata[idx].xoff * scale;
        ypos = y - (cdata[idx].yoff * scale); // Top 
        // OpenGL 0,0 is bottom-left? usually. 
        // If we want 'y' to be the baseline.
        
        // Box: 
        // Top-Left:  xpos, ypos
        // Bot-Left:  xpos, ypos - hVal
        // Top-Right: xpos + wVal, ypos
        
        float vertices[6][4] = {
            { xpos,     ypos - hVal,   s0, t1 },
            { xpos,     ypos,          s0, t0 },
            { xpos + wVal, ypos,       s1, t0 },

            { xpos,     ypos - hVal,   s0, t1 },
            { xpos + wVal, ypos,       s1, t0 },
            { xpos + wVal, ypos - hVal,s1, t1 }
        };
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += cdata[idx].xadvance * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
