#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "typedef.h"

class Texture
{
private:
    unsigned int texture;

public:
    Texture(const char* filename);
    void bind() const // 绑定纹理
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
};