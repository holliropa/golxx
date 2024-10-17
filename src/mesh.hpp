#pragma once

#include <vector>
#include "gl_common.hpp"

class Mesh {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLuint> triangles;
    GLuint VAO, VBO, EBO;

    Mesh();

    void upload();

    void clear();

    void draw();
};
