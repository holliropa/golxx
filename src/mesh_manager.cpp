#include <stdexcept>
#include "mesh_manager.hpp"

std::map<std::string, Mesh> MeshManager::Meshes;

Mesh &MeshManager::Load(const std::vector<float> &vertices,
                        const std::vector<float> &normals,
                        const std::vector<float> &uvs,
                        const std::vector<unsigned int> &triangles,
                        const std::string &name) {
    Mesh mesh;
    mesh.vertices = vertices;
    mesh.normals = normals;
    mesh.uvs = uvs;
    mesh.triangles = triangles;

    mesh.upload();

    Meshes[name] = mesh;
    return Meshes[name];
}

Mesh &MeshManager::Get(const std::string &name) {
    if (Meshes.find(name) == Meshes.end())
        throw std::out_of_range("Mesh '" + name + "' not found in MeshManager.");

    return Meshes[name];
}

void MeshManager::Clear() {
    for (const auto &iter: Meshes) {
        const Mesh &mesh = iter.second; // Reference to the current mesh

        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }

    Meshes.clear();
}