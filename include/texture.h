#pragma once
#include "typedef.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture
{
private:
    unsigned int texture;

public:
    Texture(const char* filename);
    Texture(const Image& image);
    void bind() const // 绑定纹理
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void unbind() const // 解绑纹理
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};