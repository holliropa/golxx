#include "shader.hpp"
#include "not_defined.hpp"
#include <iostream>

void checkCompileErrors(unsigned int shader, const std::string &type);

Shader::Shader() : ID{0} {
}

void Shader::compile(const char *vsCode, const char *fsCode) {
    GLuint vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glLinkProgram(0);
}

void Shader::use() {
    glUseProgram(this->ID);
}

void Shader::setInt(const char *location, GLint value) {
    glUniform1i(glGetUniformLocation(this->ID, location), value);
}

void Shader::setMatrix4fv(const char *location, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, location), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setFloat(const char *location, GLfloat value) {
    glUniform1f(glGetUniformLocation(this->ID, location), value);
}

void Shader::setVec4f(const char *location, glm::vec4 value) {
    glUniform4f(glGetUniformLocation(this->ID, location), value.x, value.y, value.z, value.w);
}

void Shader::setVec3f(const char *location, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(this->ID, location), value.x, value.y, value.z);
}

void checkCompileErrors(unsigned int shader, const std::string &type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << '\n';
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << '\n';
        }
    }
}