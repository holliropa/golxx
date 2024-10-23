#include "mesh.hpp"
#include "glm_common.hpp"

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
} Vertex;

Mesh::Mesh() : VAO(0), VBO(0), EBO(0) {
}

void Mesh::upload() {
    // Generate buffers if they have not been created yet
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    std::vector<Vertex> data;
    for (unsigned int i = 0; i < vertices.size() / 3; ++i) {
        Vertex vertex;

        vertex.position = glm::vec3(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
        vertex.normal = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
        vertex.uv = glm::vec2(uvs[i * 2], uvs[i * 2 + 1]);

        data.push_back(vertex);
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), data.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, uv));

    glBindVertexArray(0);

}

void Mesh::clear() {
    vertices.clear();
    normals.clear();
    uvs.clear();
    triangles.clear();
}

void Mesh::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::DrawInstanced(int instanceCount) {
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, (GLvoid *) 0, instanceCount);
    glBindVertexArray(0);
}

GLuint Mesh::GenerateInstanceBuffer() {
    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    instanceVBOs.push_back(instanceVBO);
    return instanceVBO;
}

void Mesh::SetInstanceBufferAttributes(GLuint buffer,
                                       const std::vector<AttributeLayout> &layout) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBindVertexArray(VAO); // Bind the VAO to set attributes
    for (const auto &attrib: layout) {
        glEnableVertexAttribArray(attrib.index);
        glVertexAttribPointer(attrib.index,
                              attrib.size,
                              attrib.type,
                              attrib.normalized,
                              attrib.stride,
                              (void *) attrib.offset);
        glVertexAttribDivisor(attrib.index, attrib.divisor);
    }
    glBindVertexArray(0); // Unbind the VAO after setting attributes

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::PutInstanceBufferData(GLuint buffer,
                                 GLsizei dataSize,
                                 const void *data,
                                 glfwxx::BufferUsage usage) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, static_cast<GLenum>(usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Mesh::addInstanceBuffer(const std::vector<AttributeLayout> &layout, GLsizei dataSize, const void *data,
                               glfwxx::BufferUsage usage) {
    GLuint instanceVBO;

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, static_cast<GLenum>(usage));

    glBindVertexArray(VAO);

    for (const auto &attrib: layout) {
        glEnableVertexAttribArray(attrib.index);
        glVertexAttribPointer(attrib.index,
                              attrib.size,
                              attrib.type,
                              attrib.normalized,
                              attrib.stride,
                              (void *) attrib.offset);
        glVertexAttribDivisor(attrib.index, attrib.divisor);
    }

    glBindVertexArray(0);

    instanceVBOs.push_back(instanceVBO);

    return instanceVBO;
}

void Mesh::updateInstanceBuffer(GLuint instanceVBO, GLsizei dataSize, const void *data) {
    // Bind the instance VBO and update its data
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);  // Update buffer with new data
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind the buffer

}
