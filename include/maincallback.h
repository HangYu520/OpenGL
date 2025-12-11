#pragma once
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "nlohmann/json.hpp"

// * 主函数中需要用到的 全局变量与 callback 函数

/*
* ---------------------
* 全局变量定义
* ---------------------
*/
const unsigned int SCR_WIDTH = 1024; // 窗口宽度
const unsigned int SCR_HEIGHT = 768; // 窗口高度

bool firstMouse = true; // 是否第一次移动鼠标
float lastX =  SCR_WIDTH / 2.0; // 鼠标上一帧的位置
float lastY =  SCR_HEIGHT / 2.0; // 鼠标上一帧的位置

float deltaTime = 0.0f;	// 当前帧和上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

float rotation = 0.0f; // 物体旋转角度
float fov = 45.0f; // 视锥体的 FOV

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); // 定义摄像机对象
Camera initialCamera = camera; // 创建一个副本保存初始状态

LightMode lightMode = LightMode::POINT;

// 立方体顶点数据
float cubeVertices[] = {
    // positions          // normals           // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

// 立方体位置
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

/*
* ---------------------
* 视口变换处理
* ---------------------
*/
inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height); // 视口变换, NDC 坐标变换到屏幕坐标
}

/*
* ---------------------
* 键盘输入处理
* ---------------------
*/
static void processInput(GLFWwindow* window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 检测 ESC 键, 退出程序
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 1.f * deltaTime;
    // 控制相机
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.adjustCameraPos(Camera::Movement::FORWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.adjustCameraPos(Camera::Movement::BACKWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.adjustCameraPos(Camera::Movement::LEFT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.adjustCameraPos(Camera::Movement::RIGHT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.adjustCameraPos(Camera::Movement::UP, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.adjustCameraPos(Camera::Movement::DOWN, cameraSpeed);
    // 控制物体旋转
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotation -= cameraSpeed * 10.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotation += cameraSpeed * 10.0f;
    // 重置相机
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {   
        camera = initialCamera;
        firstMouse = true;
        lastX =  SCR_WIDTH / 2.0;
        lastY =  SCR_HEIGHT / 2.0;
        rotation = 0.0f;
        fov = 45.0f;
    }
    // 控制灯光
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        lightMode = LightMode::DIRECTION;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        lightMode = LightMode::POINT;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        lightMode = LightMode::SPOT;
}

/*
* ---------------------
* 鼠标输入处理
* ---------------------
*/
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    // 仅在鼠标右键按下时处理视角旋转（实现拖拽效果）
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) 
    {
        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.adjustYaw(xoffset);
        camera.adjustPitch(yoffset);

        camera.adjustCameraFront();
    }
}

/*
* ---------------------
* 鼠标滚轮处理
* ---------------------
*/
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float) yoffset;
    if (fov < 1.0f) fov = 1.0f;
    else if (fov > 90.0f) fov = 90.0f;
}

/*
* ------------------------------------------
* 加载多个模型及对应的纹理、法向贴图
* ------------------------------------------
*/
struct Asset
{
    Model model;
    Image diffuse_map;
    Image specular_map;
};
inline void loadAsset(const char* input_json, std::vector<Asset>& assets)
{
    // 使用 json 文件多个模型
    using json = nlohmann::json;
    json data = json::parse(std::ifstream(input_json));
    for (auto& model: data["model"])
    {
        bool load = model.value("load", true);
        if (load)
        {
            Model objmodel;
            Image diffuse_map, specular_map;
            const std::string name = model.value("name", "");
            const std::string obj_file = model.value("obj", "");
            const std::string diffuse_map_file = model.value("diffuse", "");
            const std::string specular_map_file = model.value("specular", "");
            objmodel.loadFrom(obj_file.c_str());
            if (!diffuse_map_file.empty())
            {
                diffuse_map.load(diffuse_map_file.c_str());
                diffuse_map.flipVertical();
            }
            if (!specular_map_file.empty())
            {
                specular_map.load(specular_map_file.c_str());
                specular_map.flipVertical();
            }
            assets.push_back({objmodel, diffuse_map, specular_map});
        }
    }
}