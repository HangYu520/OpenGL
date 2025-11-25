#include "vertexarray.h"

VertexArray::VertexArray(const float* vertices, 
                         int vertexCount, 
                         int vertexAttribDim, 
                         std::initializer_list<int> vertexAttribs, 
                         const int* indices, 
                         int indexCount, 
                         int indexDim)
{
    // 创建和绑定VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // 创建和绑定VBO，并传输顶点数据
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 vertexCount * vertexAttribDim * sizeof(float), 
                 vertices, GL_STATIC_DRAW);
    
    // 如果提供了索引数据，则创建和绑定EBO，并传输索引数据
    if (indices)
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                     indexCount * indexDim * sizeof(int), 
                     indices, GL_STATIC_DRAW);
    }

    // 设置顶点属性指针
    int i = 0, offset = 0;
    for (auto iter = vertexAttribs.begin(); iter != vertexAttribs.end(); iter++)
    {
        glVertexAttribPointer(i, *iter, GL_FLOAT, GL_FALSE, 
                              vertexAttribDim * sizeof(float), 
                              (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += *iter;
        i++;
    }
    
    // 解绑VAO
    glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
    // 清除顶点数据缓存
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}