#pragma once
#include "shader.h"

// * 主函数中需要用到的 callback 函数

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
* 键盘鼠标输入处理
* ---------------------
*/
inline void processInput(GLFWwindow* window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 检测 ESC 键, 退出程序
        glfwSetWindowShouldClose(window, true);
}