#pragma once
#include "../libs/glad/glad.h"
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader() = default;

    void use() const;

    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    unsigned int getID() const { return ID; }

private:
    unsigned int ID = 0;
    void checkCompileErrors(unsigned int shader, const std::string& type);
};
