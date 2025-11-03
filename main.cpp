#include <spdlog/spdlog.h>
#include "maincallback.h"

/*
* ---------------------
* 着色器字符串
* ---------------------
*/

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";


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

    // 创建顶点着色器并编译
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 顶点着色器源码
    glCompileShader(vertexShader); // 编译顶点着色器
    int success; // 检查编译结果
    char infoLog[512]; // 获取错误信息
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // 检查编译结果
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        spdlog::error("VertexShader compliation failed : {}", infoLog);
    }

    // 创建片段着色器并编译
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 创建片段着色器
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // 片段着色器源代码
    glCompileShader(fragmentShader); // 编译片段着色器
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // 获取片段着色器的编译状态
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        spdlog::error("FragmentShader compliation failed : {}", infoLog);
    }
    
    // 创建着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram(); // 创建着色器程序
    glAttachShader(shaderProgram, vertexShader); // 添加顶点着色器
    glAttachShader(shaderProgram, fragmentShader); // 添加片段着色器
    glLinkProgram(shaderProgram); // 链接不同着色器的输入输出
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // 获取链接状态
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        spdlog::error("ShaderProgram link failed : {}", infoLog);
    }
    glDeleteShader(vertexShader); // 删除着色器
    glDeleteShader(fragmentShader);

    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 创建顶点属性指针
    glEnableVertexAttribArray(0); // 启用顶点属性

    // * 主循环
    while(!glfwWindowShouldClose(window)) 
    {
        processInput(window); // 处理键盘输入
        
        // TODO 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 指定清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清屏, 否则一直绘制的上一帧

        glUseProgram(shaderProgram); // 使用着色器程序
        glBindVertexArray(VAO); // 绑定顶点数组对象
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制
        
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents(); // 检查事件
    }

    // 清除顶点数据缓存
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}