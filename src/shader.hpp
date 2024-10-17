#pragma once

#include <string>
#include "gl_common.hpp"
#include "glm_common.hpp"

class Shader {
public:
    GLuint ID;

    Shader();

    void compile(const char *vsCode, const char *fsCode);

    void use();

    void setMatrix4fv(const char *location, const glm::mat4 &matrix) const;

    void setInt(const char *location, GLint value);

    void setFloat(const char *location, GLfloat value);

    void setVec4f(const char *location, glm::vec4 value);

    void setVec3f(const char *location, glm::vec3 value);
};