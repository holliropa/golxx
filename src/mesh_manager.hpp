#pragma once

#include <map>
#include <string>
#include <vector>
#include "mesh.hpp"

class MeshManager {
public:
    static std::map<std::string, Mesh> Meshes;

    static Mesh &Load(const std::vector<float> &vertices,
                      const std::vector<float> &normals,
                      const std::vector<float> &uvs,
                      const std::vector<unsigned int> &triangles,
                      const std::string &name);

    static Mesh &Get(const std::string &name);

    static void Clear();

private:
    MeshManager() {}
};