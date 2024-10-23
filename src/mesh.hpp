#pragma once

#include <vector>
#include "gl_common.hpp"
#include "attribute_layout.hpp"
#include "buffer_usage.hpp"

class Mesh {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLuint> triangles;
    GLuint VAO, VBO, EBO;
    std::vector<GLuint> instanceVBOs;

    Mesh();

    void upload();

    void clear();

    void draw();

    void DrawInstanced(int instanceCount);

    GLuint GenerateInstanceBuffer();

    void SetInstanceBufferAttributes(GLuint buffer,
                                     const std::vector<AttributeLayout> &layout);

    void PutInstanceBufferData(GLuint buffer,
                               GLsizei dataSize,
                               const void *data,
                               glfwxx::BufferUsage usage);

    GLuint addInstanceBuffer(const std::vector<AttributeLayout> &layout,
                             GLsizei dataSize,
                             const void *data,
                             glfwxx::BufferUsage usage);

    void updateInstanceBuffer(GLuint instanceVBO, GLsizei dataSize, const void *data);
};
