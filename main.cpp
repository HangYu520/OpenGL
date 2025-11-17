#include "maincallback.h"
#include "typedef.h"

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if(!window) 
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // 绑定窗口
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 绑定窗口大小回调函数
    glfwSetCursorPosCallback(window, mouse_callback); // 绑定鼠标回调函数
    glfwSetScrollCallback(window, scroll_callback); // 绑定鼠标滚轮回调函数

    // 初始化 Glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        spdlog::error("Failed to initialize GLAD");
        return -1;
    }
    
    // 从文件中创建着色器
    Shader lightingShader("shader/LightingShader.vs", "shader/LightingShader.fs"); // 创建物体着色器
    Shader lightcubeShader("shader/LightcubeShader.vs", "shader/LightcubeShader.fs"); // 创建光照立方体

    // 创建顶点属性和索引缓冲 VAO VBO EBO (GPU 缓存的数据内存)
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO); // 创建 VAO
    glGenBuffers(1, &VBO); // 创建 VBO
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO); // 绑定 VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定 VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定 EBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); // 传入顶点数据给 VBO
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 传入索引数据给 EBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0); // 创建顶点位置属性指针
    glEnableVertexAttribArray(0); // 启用顶点位置属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float))); // 创建顶点纹理属性指针
    glEnableVertexAttribArray(1); // 启用顶点纹理属性
    glBindVertexArray(0); // 解绑 VAO

    unsigned int VBOlight, VAOlight;
    glGenVertexArrays(1, &VAOlight);
    glGenBuffers(1, &VBOlight);
    glBindVertexArray(VAOlight);
    glBindBuffer(GL_ARRAY_BUFFER, VBOlight);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // 解绑 VAO
    #if false
    // 创建第一个纹理
    unsigned int texture0;
    glGenTextures(1, &texture0); // 创建纹理对象
    glBindTexture(GL_TEXTURE_2D, texture0); // 绑定纹理对象
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理坐标的S轴的纹理模式	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置纹理坐标的T轴的纹理模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 设置纹理的放大缩小模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理的放大缩小模式

    Image image0; image0.load("asset/wall.jpg"); // 加载纹理图片
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image0.width, image0.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image0.image_buffer); // 纹理图片数据
    glGenerateMipmap(GL_TEXTURE_2D); // 生成MipMap

    // 创建第二个纹理
    unsigned int texture1;
    glGenTextures(1, &texture1); // 创建纹理对象
    glBindTexture(GL_TEXTURE_2D, texture1); // 绑定纹理对象
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // 设置纹理坐标的S轴的纹理模式	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 设置纹理坐标的T轴的纹理模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 设置纹理的放大缩小模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 设置纹理的放大缩小模式

    Image image1; image1.load("asset/awesomeface.png"); image1.flipVertical(); // 加载纹理图片
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image1.width, image1.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.image_buffer); // 纹理图片数据
    glGenerateMipmap(GL_TEXTURE_2D); // 生成MipMap

    lightingShader.use();
    lightingShader.setInt("texture0", 0); // 设置纹理单元
    lightingShader.setInt("texture1", 1);
    #endif
    // 矩阵变换
    glm::mat4 model, view, projection;

    glm::mat4 lightmodel = glm::mat4(1.0f);
    lightmodel = glm::translate(lightmodel, lightPos);
    lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));

    glEnable(GL_DEPTH_TEST); // 启用深度测试
    
    // * 主循环
    while(!glfwWindowShouldClose(window)) 
    {
        processInput(window); // 处理键盘输入
        
        // TODO 渲染
        glClearColor(0.f, 0.f, 0.f, 1.0f); // 指定清屏颜色 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清屏, 否则一直绘制的上一帧

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame; // 计算当前帧与上一帧的时间差
        lastFrame = currentFrame;  
        
        // 根据键盘鼠标更新变换矩阵
        model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(fov), SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
        
        // 设置着色器全局变量
        lightingShader.use(); // 使用着色器程序
        lightingShader.setMVP(model, view, projection);
        lightingShader.setVec3("objColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        #if false
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        #endif
        glBindVertexArray(VAO); // 绑定物体数组对象
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        lightcubeShader.use(); // 光源立方体
        lightcubeShader.setMVP(lightmodel, view, projection);
        glBindVertexArray(VAOlight); // 绑定光源数组对象
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents(); // 检查事件
    }

    // 清除顶点数据缓存
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
}