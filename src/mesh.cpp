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