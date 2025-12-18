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
    Shader lightingShader("shader/SimpleShader.vs", "shader/SimpleShader.fs"); // 创建物体着色器

    // 创建 VAO
    VertexArray cubeVAO(cubeVertices, 36, 8, { 3, 3, 2 });
    VertexArray floorVAO(planeVertices, 6, 8, { 3, 3, 2 });
    
    // 创建纹理
    Texture cubeTexture("asset/marble.jpg");
    Texture floorTexture("asset/metal.png");

    lightingShader.use();
    lightingShader.setInt("texture1", 0);
    
    // 矩阵变换
    glm::mat4 model, view, projection;
    
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
        model = glm::rotate(
            glm::mat4(1.0f), 
            glm::radians(rotation), 
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        projection = glm::perspective(
            glm::radians(fov), 
            SCR_WIDTH / (float) SCR_HEIGHT, 
            0.1f, 100.0f
        );
        view = camera.getViewMatrix();
        
        // 绘制地板
        lightingShader.use();
        glActiveTexture(GL_TEXTURE0); floorTexture.bind();
        floorVAO.bind();
        auto model_floor = glm::mat4(1.0f);
        lightingShader.setMVP(model_floor, view, projection);
        glDrawArrays(GL_TRIANGLES, 0, floorVAO.vertexCount);
        floorVAO.unbind();

        // 绘制物体
        lightingShader.use();
        glActiveTexture(GL_TEXTURE0); cubeTexture.bind();
        cubeVAO.bind();
        // 绘制第一个立方体
        auto model_1 = glm::translate(model, glm::vec3(-1.0f, 1e-3f, -1.0f)); 
        lightingShader.setMVP(model_1, view, projection);
        glDrawArrays(GL_TRIANGLES, 0, cubeVAO.vertexCount);
        // 绘制第二个立方体
        auto model_2 = glm::translate(model, glm::vec3(2.0f, 1e-3f, 0.0f)); 
        lightingShader.setMVP(model_2, view, projection);
        glDrawArrays(GL_TRIANGLES, 0, cubeVAO.vertexCount);
        cubeVAO.unbind();
        
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents(); // 检查事件
    }

    glfwTerminate();
}