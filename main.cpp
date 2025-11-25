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
    Shader lightingShader("shader/PhongShader.vs", "shader/PhongShader.fs"); // 创建物体着色器
    Shader lightcubeShader("shader/LightcubeShader.vs", "shader/LightcubeShader.fs"); // 创建光照立方体

    // 创建 VAO
    VertexArray vaoObj(cubeVertices, 36, 8, { 3, 3, 2 }); // 创建顶点数组对象
    VertexArray vaoLight(cubeVertices, 36, 8, { 3 }); // 创建光源数组对象
    
    // 创建纹理
    Texture diffuse_map("asset/container2.png");
    Texture specular_map("asset/container2_specular.png");

    // 设置纹理单元
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    
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
        glm::vec3 cameraPos = camera.cameraPos;
        
        // 绘制物体
        lightingShader.use(); // 使用着色器程序
        lightingShader.setMVP(model, view, projection);
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
        
        lightingShader.setInt("lightMode", (int) lightMode);
        if (lightMode == LightMode::DIRECTION) lightingShader.setLight("dirLight", DirectionLight::getCase());
        if (lightMode == LightMode::POINT) lightingShader.setLight("pointLight", PointLight::getCase());
        if (lightMode == LightMode::SPOT) lightingShader.setLight("spotLight", SpotLight::getCase());
        
        glActiveTexture(GL_TEXTURE0); diffuse_map.bind();
        glActiveTexture(GL_TEXTURE1); specular_map.bind();
        
        vaoObj.bind(); 
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(rotation + angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        vaoObj.unbind();
        
        // 绘制光源
        if (lightMode == LightMode::POINT)
        {
            lightcubeShader.use();
            vaoLight.bind();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, PointLight::getCase().position);
            model = glm::scale(model, glm::vec3(0.2f));
            lightcubeShader.setMVP(model, view, projection);
            glm::vec3 lightColor = PointLight::getCase().lightColor;
            lightcubeShader.setVec3("color", lightColor.x, lightColor.y, lightColor.z);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            vaoLight.unbind();
        }
        
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents(); // 检查事件
    }

    glfwTerminate();
}