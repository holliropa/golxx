#include <stdexcept>
#include "shader_manager.hpp"

std::map<std::string, Shader> ShaderManager::Shaders;

Shader &ShaderManager::Load(const char *vsCode, const char *fsCode, const std::string &name) {
    Shader shader;
    shader.compile(vsCode, fsCode);
    Shaders[name] = shader;
    return Shaders[name];
}

Shader &ShaderManager::Get(const std::string &name) {
    if (Shaders.find(name) == Shaders.end())
        throw std::out_of_range("Shader '" + name + "' not found in ResourceManager.");

    return Shaders[name];
}

void ShaderManager::Clear() {
    for (const auto &iter: Shaders) {
        glDeleteProgram(iter.second.ID);
    }

    Shaders.clear();
}
