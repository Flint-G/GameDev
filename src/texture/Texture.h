#pragma once
#include "../../libs/glad/glad.h"
#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture();

    bool loadFromFile(const std::string& path, bool flipVertically = true);

    void bind(unsigned int unit = 0) const;
    void unbind() const;

    int getWidth()  const { return width; }
    int getHeight() const { return height; }
    inline unsigned int getID() const { return textureID; }

private:
    unsigned int textureID = 0;
    int width;
    int height;
};
