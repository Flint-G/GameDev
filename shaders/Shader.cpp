#include "Shader.h"
#include <iostream>

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    // Vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vSrc = vertexSrc.c_str();
    glShaderSource(vertex, 1, &vSrc, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSrc = fragmentSrc.c_str();
    glShaderSource(fragment, 1, &fSrc, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    if (ID != 0)
        glUseProgram(ID);
    else
        std::cerr << "Warning: Invalid shader program!\n";
}

void Shader::setFloat(const std::string& name, float value) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
        glUniform1f(loc, value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    int loc = glGetUniformLocation(ID, name.c_str());
    if (loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "Shader compilation error (" << type << "): " << infoLog << "\n";
            ID = 0;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "Shader linking error (" << type << "): " << infoLog << "\n";
            ID = 0;
        }
    }
}
