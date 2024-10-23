#pragma once

#include "gl_common.hpp"

struct AttributeLayout {
    GLuint index;       // Attribute location in the shader
    GLint size;         // Number of components (e.g., 1 for float, 3 for vec3, etc.)
    GLenum type;        // Data type (GL_FLOAT, GL_INT, etc.)
    GLboolean normalized; // Should the data be normalized (usually GL_FALSE)
    GLsizei stride;     // The size of a single instance of the data structure (in bytes)
    size_t offset;      // The byte offset within the data structure
    GLuint divisor;     // How often to update the attribute (1 for per-instance)
};
