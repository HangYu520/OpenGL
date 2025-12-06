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

    // 载入模型
    std::vector<Asset> assets;
    loadAsset("asset/model.json", assets);

    // 创建 VAO
    VertexArray vaoLight(cubeVertices, 36, 8, { 3 }); // 创建光源数组对象
    std::vector<VertexArray> vaoArr;
    vaoArr.reserve(assets.size());
    for (auto& asset : assets) 
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        asset.model.getBuffer(vertices, indices);
        std::initializer_list<int> layout = { 3, 3, 2 };
        vaoArr.emplace_back(
            vertices.data(), vertices.size() / 8, 8, layout, 
            indices.data(), asset.model.num_faces(), 3);
    }
    
    // 创建纹理
    std::vector<Texture> diffuseArr, specularArr;
    for (auto& asset : assets) 
    {
        diffuseArr.emplace_back(asset.diffuse_map);
        specularArr.emplace_back(asset.specular_map);
    }

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
        
        // 绘制物体
        lightingShader.use(); // 使用着色器程序
        lightingShader.setMVP(model, view, projection);
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setVec3("cameraPos", camera.cameraPos);
        
        lightingShader.setInt("lightMode", (int) lightMode);
        if (lightMode == LightMode::DIRECTION) lightingShader.setLight("dirLight", DirectionLight::getCase());
        if (lightMode == LightMode::POINT) lightingShader.setLight("pointLight", PointLight::getCase());
        if (lightMode == LightMode::SPOT) lightingShader.setLight("spotLight", SpotLight::getCase());

        for(int i = 0; i < vaoArr.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0); diffuseArr[i].bind();
            glActiveTexture(GL_TEXTURE1); specularArr[i].bind();

            vaoArr[i].bind();
            glDrawElements(GL_TRIANGLES, 3 * vaoArr[i].indexCount, GL_UNSIGNED_INT, 0);
            vaoArr[i].unbind();
        }
        
        // 绘制光源
        if (lightMode == LightMode::POINT)
        {
            lightcubeShader.use();
            vaoLight.bind();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, PointLight::getCase().position);
            model = glm::scale(model, glm::vec3(0.2f));
            lightcubeShader.setMVP(model, view, projection);
            lightcubeShader.setVec3("color", PointLight::getCase().lightColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            vaoLight.unbind();
        }
        
        glfwSwapBuffers(window); // 交换缓冲
        glfwPollEvents(); // 检查事件
    }

    glfwTerminate();
}