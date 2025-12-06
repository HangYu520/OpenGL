#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <initializer_list>

class VertexArray
{
private:
    unsigned int VAO, VBO, EBO;

public:
    int vertexCount, indexCount;

public:
    VertexArray() = default;

    VertexArray(
        const float* vertices, // 顶点数据
        int vertexCount, // 顶点数量
        int vertexAttribDim, // 顶点属性总维度
        std::initializer_list<int> vertexAttribs, // 每个顶点属性的维度
        const unsigned int* indices = nullptr, // 索引数据
        int indexCount = 0, // 索引数量
        int indexDim = 3 // 索引维度
    );
    
    ~VertexArray();

    // 绑定和解绑
    void bind() const { glBindVertexArray(VAO); }

    void unbind() const { glBindVertexArray(0); }
};