#pragma once
#include <random>
#include <fstream>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>
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
* ---------------------------------
* 2. 3D 模型类 (封装 tinyobjloader)
* ---------------------------------
*/
class Model // * 
{
    // * 数据类型
public:
    struct attrib_t
    {
        // 属性类型定义
        struct position     { float x, y, z; };// 位置结构体
        struct normal       { float nx, ny, nz;}; // 法线结构体
        struct texcoord     { float u, v; };// 纹理坐标结构体
        // TODO : 添加颜色等其他属性
        position            _position;
        normal              _normal;
        texcoord            _texcoord;
    };
    using line_attrib_t     = std::tuple<attrib_t, attrib_t>; // 线段属性类型定义
    using triface_attrib_t  = std::tuple<attrib_t, attrib_t, attrib_t>; // 三角形面属性类型定义
    using bounding_box_t    = std::tuple<attrib_t::position, attrib_t::position>; // 包围盒类型定义 (最小点, 最大点)
    
    struct Attrib
    {
        // 属性列表 （分开存储, 因为个数可能不同)
        std::vector<attrib_t::position>   _vertices; // 位置列表
        std::vector<attrib_t::normal>     _normals; // 法线列表
        std::vector<attrib_t::texcoord>   _texcoords; // 纹理坐标列表
    };
    struct point_t // 顶点结构体
    {
        int                     vertex_index;
        int                     normal_index;
        int                     texcoord_index;
        const attrib_t          Get(const Attrib& attrib) const; // 获取 attrib 中对应的顶点属性 (常量版本)
    };
    struct line_t // 线段结构体
    {
        point_t                 start;
        point_t                 end;
        const line_attrib_t     Get(const Attrib& attrib) const; // 获取 attrib 中对应的线段顶点属性 (常量版本)
    };
    struct triface_t // 三角形面结构体
    {
        point_t                 vertex[3]; // 三个顶点索引
        const triface_attrib_t  Get(const Attrib& attrib) const; // 获取 attrib 中对应的三角形面顶点属性 (常量版本)
    };
    // TODO : 添加四边形面等其他面类型
    
    using Points    =   std::vector<point_t>; // 点列表类型定义
    using Lines     =   std::vector<line_t>; // 线段列表类型定义
    using Trifaces  =   std::vector<triface_t>; // 三角形面列表类型定义

    struct TriMeshes // 三角形网格结构体
    {
        Trifaces trifaces;
        std::vector<int> material_ids;  // 材质ID列表，对应每个三角形面
    };
    struct shape_t // 形状结构体
    {
        std::string     name; // 形状名称
        TriMeshes       trimeshes; // 形状包含的 mesh 列表
        Lines           lines; // 形状包含的线段列表
        Points          points; // 形状包含的点列表
    };

    // * 私有变量
private:
    tinyobj::ObjReaderConfig            readerConfig; // 读取配置
    Attrib                              attrib; // 模型属性
    std::vector<shape_t>                shapes; // 模型形状列表
    std::vector<tinyobj::material_t>    materials; // 材质列表
    
    // * 私有方法
    void loadFrom(const tinyobj::attrib_t& tinyattrib, const std::vector<tinyobj::shape_t>& tinyshapes); // tinyobj 属性值展开存储, 在此处转换为封装存储
    void loadFrom(const tinyobj::attrib_t& tinyattrib); // 从 tinyobjloader 加载属性
    void loadFrom(const std::vector<tinyobj::shape_t>& tinyshapes); // 从 tinyobjloader 加载形状列表

    // * 公有方法
public:
    /*
    * ------------------------------------------
    * 设置模型读取配置 (Load 前调用, 目前只支持三角形面)
    * ------------------------------------------
    * /inType: bool triangulate 是否三角化多边形网格
    * /inType: bool vertex_color 是否加载顶点颜色
    * /inType: std::string triangulation_method 三角化方法
    * /inType: std::string mtl_search_path 材质文件搜索路径
    * ------------------------------------------
    */
    void setLoadConfig(bool triangulate = true, bool vertex_color = true, std::string triangulation_method = "Simple", std::string mtl_search_path = "");
    /*
    * ------------------------------------------
    * 从文件加载模型 (Load obj 文件)
    * ------------------------------------------
    * /inType: const std::string& filename 文件名
    * ------------------------------------------
    */
    void loadFrom(const std::string& filename); // 从文件加载模型
    /*
    * ------------------------------------------
    * 获取模型包围盒 (最小点, 最大点)
    * ------------------------------------------
    * /outType: bounding_box_t 包围盒 (最小点, 最大点)
    * ------------------------------------------
    */
    bounding_box_t getBoundingBox() const; // 获取模型包围盒 (最小点, 最大点)
    /*
    * ------------------------------------------
    * 平移模型
    * ------------------------------------------
    * /inType: float x_offset x 方向偏移量
    * /inType: float y_offset y 方向偏移量
    * /inType: float z_offset z 方向偏移量
    * ------------------------------------------
    */
    void translate(float x_offset, float y_offset, float z_offset); // 平移模型
    /*
    * ------------------------------------------
    * 按比例缩放模型
    * ------------------------------------------
    * /inType: float x_scale x 缩放比例
    * /inType: float y_scale y 缩放比例
    * /inType: float z_scale z 缩放比例
    * ------------------------------------------
    */
    void resize(float x_scale, float y_scale, float z_scale); // 按比例缩放模型

    // 获取顶点属性
    Attrib& getAttrib() { return attrib; }

    // 获取所有形状中的三角形面列表
    std::vector<Trifaces>  getTrifaces(); 

    // 保存 OBJ 文件
    void saveOBJ(const char* filename, const std::vector<attrib_t::position>& vertices, const Trifaces& trifaces);

    // 打印模型形状信息
    void logShapes() const; 
};

/*
* -----------------------------
* 3. 光照结构体
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