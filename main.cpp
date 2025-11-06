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
    
    // 从文件中创建着色器
    Shader shader("shader/Shader.vs", "shader/Shader.fs");

    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    }; // ! 自定义的顶点数据
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; // ! 自定义的索引数据

    // 创建顶点属性和索引缓冲 VAO VBO EBO (GPU 缓存的数据内存)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // 创建 VAO
    glGenBuffers(1, &VBO); // 创建 VBO
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO); // 绑定 VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定 EBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 传入顶点数据给 VBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 传入索引数据给 EBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 创建顶点位置属性指针
    glEnableVertexAttribArray(0); // 启用顶点位置属性

    // * 主循环
    while(!glfwWindowShouldClose(window)) 
    {
        processInput(window); // 处理键盘输入
        
        // TODO 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 指定清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清屏, 否则一直绘制的上一帧
        
        // 设置着色器全局变量
        float timeValue = glfwGetTime();
        float redValue = sin(timeValue) / 2.0f + 0.5f;
        shader.setVec4("ourColor", redValue, 0.0f, 0.0f, 1.0f);
        shader.use(); // 使用着色器程序
        glBindVertexArray(VAO); // 绑定顶点数组对象
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制
        
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents(); // 检查事件
    }

    // 清除顶点数据缓存
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
}