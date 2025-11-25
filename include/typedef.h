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
* 2. 光照结构体
* -----------------------------
*/

enum class LightMode
{
    DIRECTION = 1,
    POINT = 2,
    SPOT = 3
};

struct DirectionLight // 平行光
{
    glm::vec3 direction; // 光源方向
    glm::vec3 ambient; // 环境光强度
    glm::vec3 diffuse; // 漫反射光强度
    glm::vec3 specular; // 镜面反射光强度
    glm::vec3 lightColor; // 光源颜色

    DirectionLight(
        const glm::vec3& direction,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        const glm::vec3& lightColor
    )
    : direction(direction),
      ambient(ambient * lightColor),
      diffuse(diffuse * lightColor),
      specular(specular),
      lightColor(lightColor)
    {}

    DirectionLight() = default;

    static DirectionLight getCase()
    {
        static DirectionLight dirLight = DirectionLight(
            glm::vec3(-0.2f, -1.0f, -0.3f), // direction
            glm::vec3(0.2f, 0.2f, 0.2f), // ambient
            glm::vec3(0.5f, 0.5f, 0.5f), // diffuse
            glm::vec3(1.0f, 1.0f, 1.0f),  // specular
            glm::vec3(1.0f, 1.0f, 1.0f) // lightColor
        );
        return dirLight;
    }
};

struct PointLight // 点光源
{
    glm::vec3 position; // 点光源位置
    glm::vec3 ambient; // 环境光强度
    glm::vec3 diffuse; // 漫反射光强度
    glm::vec3 specular; // 镜面反射光强度
    float constant; // 常数项
    float linear; // 线性项
    float quadratic; // 二次项
    glm::vec3 lightColor; // 光源颜色

    PointLight(
        const glm::vec3& position,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        float constant,
        float linear,
        float quadratic,
        const glm::vec3& lightColor
    )
    : position(position),
      ambient(ambient * lightColor),
      diffuse(diffuse * lightColor),
      specular(specular),
      constant(constant),
      linear(linear),
      quadratic(quadratic),
      lightColor(lightColor)
    {}

    PointLight() = default;

    static PointLight getCase()
    {
        static PointLight pointLight = PointLight(
            glm::vec3( 0.7f,  0.2f,  2.0f), // position
            glm::vec3(0.2f, 0.2f, 0.2f), // ambient
            glm::vec3(0.5f, 0.5f, 0.5f), // diffuse
            glm::vec3(1.0f, 1.0f, 1.0f), // specular
            1.0f, // constant
            0.09f, // linear
            0.032f, // quadratic
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        return pointLight;
    }
};

struct SpotLight // 聚光灯
{
    glm::vec3 position; // 聚光灯位置
    glm::vec3 direction; // 聚光灯方向
    float cutOff; // 聚光灯 cutoff
    float outerCutOff; // 聚光灯 outerCutOff
    glm::vec3 ambient; // 环境光强度
    glm::vec3 diffuse; // 漫反射光强度
    glm::vec3 specular; // 镜面反射光强度
    glm::vec3 lightColor; // 光源颜色

    SpotLight(
        const glm::vec3& position,
        const glm::vec3& direction,
        float cutOff,
        float outerCutOff,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular,
        const glm::vec3& lightColor
    )
    : position(position),
      direction(direction),
      cutOff(cutOff),
      outerCutOff(outerCutOff),
      ambient(ambient * lightColor),
      diffuse(diffuse * lightColor),
      specular(specular),
      lightColor(lightColor)
    {}

    SpotLight() = default;

    static SpotLight getCase()
    {
        static SpotLight spotLight = SpotLight(
            glm::vec3(0.0f, 0.0f, 3.0f), // position
            glm::vec3(0.0f, 0.0f, -1.0f), // direction
            glm::cos(glm::radians(12.5f)), // cutOff
            glm::cos(glm::radians(17.5f)), // outerCutOff
            glm::vec3(0.2f, 0.2f, 0.2f), // ambient
            glm::vec3(0.5f, 0.5f, 0.5f), // diffuse
            glm::vec3(1.0f, 1.0f, 1.0f), // specular
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
        return spotLight;
    }
};