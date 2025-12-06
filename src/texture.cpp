#include "texture.h"

Texture::Texture(const char* filename)
{
    glGenTextures(1, &texture); // 创建纹理对象
    glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理对象
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理坐标的S轴的纹理模式	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置纹理坐标的T轴的纹理模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 设置纹理的放大缩小模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理的放大缩小模式
    
    Image image; image.load(filename); image.flipVertical(); // 加载纹理图片
    if (image.channel == Image::RGBA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image_buffer); // 纹理图片数据
    else if (image.channel == Image::RGB)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.image_buffer); // 纹理图片数据
    else
        exit(-1);
    glGenerateMipmap(GL_TEXTURE_2D); // 生成MipMap
}

Texture::Texture(const Image& image)
{
    glGenTextures(1, &texture); // 创建纹理对象
    glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理对象
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理坐标的S轴的纹理模式	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置纹理坐标的T轴的纹理模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 设置纹理的放大缩小模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理的放大缩小模式
    
    if (image.channel == Image::RGBA)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.image_buffer); // 纹理图片数据
    else if (image.channel == Image::RGB)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.image_buffer); // 纹理图片数据
    else
        exit(-1);
    glGenerateMipmap(GL_TEXTURE_2D); // 生成MipMap
}