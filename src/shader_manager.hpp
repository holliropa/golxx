#pragma once

#include <map>
#include <string>
#include "shader.hpp"

class ShaderManager {
public:
    static std::map<std::string, Shader> Shaders;

    static Shader &Load(const char *vsCode,
                        const char *fsCode,
                        const std::string &name);

    static Shader &Get(const std::string &name);

    static void Clear();

private:
    ShaderManager() {}
};