#pragma once
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <spdlog/spdlog.h>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// * 自定义数据类型头文件

/*
* -----------------------------
* 1. 图像结构体 (封装 stb_image)
* -----------------------------
*/
struct Image
{ 
    // 数据类型
    enum Channel // 图像通道数
    {
        GRAY = 1,
        GRAY_ALPHA = 2,
        RGB = 3,
        RGBA = 4
    };
    struct Color // 颜色结构体
    {
        unsigned char R, G, B;
        static Color randColor(); // 生成随机颜色
        // 预定义颜色
        static const Color  RED;
        static const Color  GREEN;
        static const Color  BLUE;
        static const Color  WHITE;
        static const Color  BLACK;
        static const Color  YELLOW;
    };
    
    struct Pixel // 像素结构体
    {
        int x, y;
    }; 

    // 成员变量
    stbi_uc*    image_buffer; // 图像数据
    int         width, height; // 图像宽高
    Channel     channel; // 图像通道数
    Image() : image_buffer(nullptr), width(0), height(0), channel(RGBA) {};
    Image(int w, int h, Channel c); // 构造函数, 分配图像内存并初始化为0

    /*
    * ------------------------------------------
    * 设置指定像素的颜色
    * ------------------------------------------
    * /inType: const Pixel& pixel 像素坐标
    * /inType: const Color& color 颜色
    * ------------------------------------------
    */
    void setColor(const Pixel& pixel, const Color& color);

    /*
    * ------------------------------------------
    * 上下反转图像（垂直翻转）
    * ------------------------------------------
    */
    void flipVertical();

    /*
    * ------------------------------------------
    * 加载图像文件
    * ------------------------------------------
    * /inType: const char* filename 文件名
    * ------------------------------------------
    */
    void load(const char* filename); // 加载图像文件

    /*
    * ------------------------------------------
    * 保存图像文件
    * ------------------------------------------
    * /inType: const char* filename 文件名
    * ------------------------------------------
    */
    void save(const char* filename); // 写入图像文件
};

/*
* -----------------------------
* 2. 材质结构体
* -----------------------------
*/
struct Material
{
    glm::vec3 ambient; // 环境光颜色
    glm::vec3 diffuse; // 漫反射颜色
    glm::vec3 specular; // 镜面反射颜色
    float shininess; // 镜面反射指数
};

/*
* -----------------------------
* 3. 光照结构体
* -----------------------------
*/
struct Light
{
    glm::vec3 position; // 光源位置
    glm::vec3 ambient; // 环境光强度
    glm::vec3 diffuse; // 漫反射光强度
    glm::vec3 specular; // 镜面反射光强度
};