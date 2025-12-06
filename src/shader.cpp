#include "shader.h"
#include <spdlog/spdlog.h>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    // 从文件中读取顶点着色器代码
    std::string vertexCode;
    std::ifstream vShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexShaderFile);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        spdlog::error("ERROR::SHADER::VERTEX::FILE_NOT_SUCCESSFULLY_READ");
    }
    // 从文件中读取片段着色器代码
    std::string fragmentCode;
    std::ifstream fShaderFile;
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fShaderFile.open(fragmentShaderFile);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        spdlog::error("ERROR::SHADER::FRAGMENT::FILE_NOT_SUCCESSFULLY_READ");
    }

    // 创建顶点着色器并编译
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器
    const char* vertexShaderSource = vertexCode.c_str();
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
    const char* fragmentShaderSource = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // 片段着色器源代码
    glCompileShader(fragmentShader); // 编译片段着色器
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); // 获取片段着色器的编译状态
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        spdlog::error("FragmentShader compliation failed : {}", infoLog);
    }
    
    // 创建着色器程序
    m_shaderProgramID = glCreateProgram(); // 创建着色器程序
    glAttachShader(m_shaderProgramID, vertexShader); // 添加顶点着色器
    glAttachShader(m_shaderProgramID, fragmentShader); // 添加片段着色器
    glLinkProgram(m_shaderProgramID); // 链接不同着色器的输入输出
    glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success); // 获取链接状态
    if (!success)
    {
        glGetProgramInfoLog(m_shaderProgramID, 512, NULL, infoLog);
        spdlog::error("ShaderProgram link failed : {}", infoLog);
    }
    glDeleteShader(vertexShader); // 删除着色器
    glDeleteShader(fragmentShader);
}

void Shader::setLight(const std::string& name, const DirectionLight& light) const
{
    setVec3(name + ".direction", light.direction);
    setVec3(name + ".lightColor.ambient", light.ambient);
    setVec3(name + ".lightColor.diffuse", light.diffuse);
    setVec3(name + ".lightColor.specular", light.specular);
}

void Shader::setLight(const std::string& name, const PointLight& light) const
{
    setVec3(name + ".position", light.position);
    setVec3(name + ".lightColor.ambient", light.ambient);
    setVec3(name + ".lightColor.diffuse", light.diffuse);
    setVec3(name + ".lightColor.specular", light.specular);
    setFloat(name + ".constant", light.constant);
    setFloat(name + ".linear", light.linear);
    setFloat(name + ".quadratic", light.quadratic);
}

void Shader::setLight(const std::string& name, const SpotLight& light) const
{
    setVec3(name + ".position", light.position);
    setVec3(name + ".direction", light.direction);
    setFloat(name + ".cutOff", light.cutOff);
    setFloat(name + ".outerCutOff", light.outerCutOff);
    setVec3(name + ".lightColor.ambient", light.ambient);
    setVec3(name + ".lightColor.diffuse", light.diffuse);
    setVec3(name + ".lightColor.specular", light.specular);
}