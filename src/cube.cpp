#include "cube.h"

namespace golxx {
    auto cube_vertex_shader_source = R"(
#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0);
})";

    auto cube_fragment_shader_source = R"(
#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
})";

    struct CubeVertex {
        glm::vec3 position;
    };

    void Cube::init() {
        init_mesh();
        init_shaders();
    }

    void Cube::render(const bw::engine::Camera& camera) {
        glad::Bind(*shader_program_);
        glad::Bind(*vertex_array_);

        glad::UniformMat4(*shader_program_, "projection").set(glm::value_ptr(camera.get_projection()));
        glad::UniformMat4(*shader_program_, "view").set(glm::value_ptr(camera.get_view()));
        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(0.5f));
        model = glm::scale(model, glm::vec3(1.2f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));


        glad::DrawElements(
            glad::PrimitiveType::Triangles,
            6,
            glad::IndexType::UnsignedInt
        );
    }

    void Cube::init_mesh() {
        vertex_array_ = std::make_unique<glad::VertexArray>();
        array_buffer_ = std::make_unique<glad::ArrayBuffer>();
        element_array_buffer_ = std::make_unique<glad::ElementArrayBuffer>();

        const std::vector<CubeVertex> vertices = {
            {{-0.5f, -0.5f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}},
        };

        const std::vector<GLuint> indices = {
            0, 1, 2,
            0, 2, 3
        };

        glad::Bind(*vertex_array_);

        array_buffer_->data(sizeof(CubeVertex) * vertices.size(), vertices.data());
        element_array_buffer_->data(sizeof(GLuint) * indices.size(), indices.data());

        glad::VertexAttribute(0)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(CubeVertex),
                     reinterpret_cast<GLvoid*>(offsetof(CubeVertex, position)))
            .enable();
    }

    void Cube::init_shaders() {
        shader_program_ = std::make_unique<glad::Program>();

        auto vertex_shader = glad::VertexShader();
        vertex_shader.set_source(cube_vertex_shader_source);

        auto fragment_shader = glad::FragmentShader();
        fragment_shader.set_source(cube_fragment_shader_source);

        shader_program_->attach_shader(vertex_shader, fragment_shader);
        shader_program_->link();
    }
}
