#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <string>
#include <sstream>
#include "typedef.h"

class Shader
{
private:
    unsigned int m_shaderProgramID; // shader id

public:
    /*
    * -----------------------------------
    *  从文件构建着色器对象
    * -----------------------------------
    * @param vertexShaderFile   顶点着色器文件名
    * @param fragmentShaderFile 片段着色器文件名
    * -----------------------------------
    */
    Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
    ~Shader()
    {
        glDeleteProgram(m_shaderProgramID);
    }

    void use() const
    {
        glUseProgram(m_shaderProgramID);
    }

    // 设置着色器的全局变量
    void setBool(const std::string& name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), (int)value); 
    }
    void setInt(const std::string& name, int value) const
    { 
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value); 
    }
    void setFloat(const std::string& name, float value) const
    { 
        glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value); 
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y, z);
    } 
    void setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y, z, w);
    }
    void setMat4(const std::string& name, const glm::mat4& trans) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
    }
    void setMVP(const glm::mat4& modeltrans, const glm::mat4& viewtrans, const glm::mat4& projtrans) const
    {
        setMat4("model", modeltrans);
        setMat4("view", viewtrans);
        setMat4("projection", projtrans);
    }
    // 设置着色器的光源
    void setLight(const DirectionLight& light) const
    {
        setVec3("light.direction", light.direction.x, light.direction.y, light.direction.z);
        setVec3("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
        setVec3("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
        setVec3("light.specular", light.specular.x, light.specular.y, light.specular.z);
    }
    void setLight(const PointLight& light) const
    {
        setVec3("light.position", light.position.x, light.position.y, light.position.z);
        setVec3("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
        setVec3("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
        setVec3("light.specular", light.specular.x, light.specular.y, light.specular.z);
        setFloat("light.constant", light.constant);
        setFloat("light.linear", light.linear);
        setFloat("light.quadratic", light.quadratic);
    }
    void setLight(const SpotLight& light) const
    {
        setVec3("light.position", light.position.x, light.position.y, light.position.z);
        setVec3("light.direction", light.direction.x, light.direction.y, light.direction.z);
        setFloat("light.cutOff", light.cutOff);
        setFloat("light.outerCutOff", light.outerCutOff);
        setVec3("light.ambient", light.ambient.x, light.ambient.y, light.ambient.z);
        setVec3("light.diffuse", light.diffuse.x, light.diffuse.y, light.diffuse.z);
        setVec3("light.specular", light.specular.x, light.specular.y, light.specular.z);
    }
};