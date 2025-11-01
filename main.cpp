#include <spdlog/spdlog.h>
#include "maincallback.h"

/*
* ---------------------
* 主函数入口，窗口渲染循环
* ---------------------
*/
int main() 
{
    // 配置 GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // * 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(!window) 
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // 绑定窗口
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 绑定窗口大小回调函数

    // 初始化 Glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }

    // * 主循环
    while(!glfwWindowShouldClose(window)) 
    {
        processInput(window); // 处理键盘输入
        
        // TODO 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 指定清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清屏, 否则一直绘制的上一帧
        
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents(); // 检查事件
    }

    glfwTerminate();
    return 0;
}