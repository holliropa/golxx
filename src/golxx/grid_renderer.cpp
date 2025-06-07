#include "golxx/grid_renderer.h"

namespace golxx {
    auto vertex_shader_source = R"(
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in vec2 a_instance_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 position = a_position + vec3(a_instance_position, 0.0);

    gl_Position = projection * view * model * vec4(position, 1.0);
})";

    auto fragment_shader_source = R"(
#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
})";

    struct CellVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct CellInstanceData {
        glm::vec2 position;
    };

    void GridRenderer::init() {
        init_mesh();
        init_shaders();
    }

    void GridRenderer::update(float deltaTime) {}


    void GridRenderer::render(const std::shared_ptr<Camera>& camera) {
        glad::Bind(*shader_program_);
        glad::UniformMat4(*shader_program_, "projection").set(glm::value_ptr(camera->get_projection()));
        const auto view = glm::translate(glm::identity<glm::mat4>(), -camera->position);
        glad::UniformMat4(*shader_program_, "view").set(glm::value_ptr(view));
        auto model = glm::identity<glm::mat4>();
        model = glm::scale(model, glm::vec3(cell_size_, cell_size_, 1.0f));
        model = glm::translate(model, glm::vec3(0.5f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));

        std::vector<CellInstanceData> cells(simulator_->getCells().size());
        unsigned i = 0;
        for (const auto& liveCell : simulator_->getCells()) {
            cells[i++] = {
                .position = {
                    static_cast<float>(liveCell.x) * cell_size_,
                    static_cast<float>(liveCell.y) * cell_size_,
                },
            };
        }

        instance_array_buffer_->data(
            sizeof(CellInstanceData) * cells.size(),
            cells.data(),
            glad::BufferUsage::DynamicDraw
        );

        glad::Bind(*vertex_array_);
        glad::DrawElementsInstanced(
            glad::PrimitiveType::Triangles,
            6,
            glad::IndexType::UnsignedInt,
            cells.size()
        );
    }

    void GridRenderer::init_mesh() {
        vertex_array_ = std::make_unique<glad::VertexArray>();
        array_buffer_ = std::make_unique<glad::ArrayBuffer>();
        instance_array_buffer_ = std::make_unique<glad::ArrayBuffer>();
        element_array_buffer_ = std::make_unique<glad::ElementArrayBuffer>();

        const std::vector<CellVertex> cell_vertices = {
            {
                {-0.5f, -0.5f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                {0.0f, 0.0f}
            },
            {
                {0.5f, -0.5f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                {1.0f, 0.0f}
            },
            {
                {0.5f, 0.5f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                {1.0f, 1.0f}
            },
            {
                {-0.5f, 0.5f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                {0.0f, 1.0f}
            },
        };

        const std::vector<GLuint> indices = {
            0, 1, 2,
            0, 2, 3
        };

        glad::Bind(*vertex_array_);

        array_buffer_->data(sizeof(CellVertex) * cell_vertices.size(), cell_vertices.data());
        element_array_buffer_->data(sizeof(GLuint) * indices.size(), indices.data());

        glad::VertexAttribute(0)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(CellVertex),
                     reinterpret_cast<GLvoid*>(offsetof(CellVertex, position)))
            .enable();
        glad::VertexAttribute(1)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(CellVertex),
                     reinterpret_cast<GLvoid*>(offsetof(CellVertex, normal)))
            .enable();
        glad::VertexAttribute(2)
            .pointer(2,
                     glad::DataType::Float,
                     false,
                     sizeof(CellVertex),
                     reinterpret_cast<GLvoid*>(offsetof(CellVertex, uv)))
            .enable();

        glad::Bind(*instance_array_buffer_);
        glad::VertexAttribute(3)
            .pointer(2,
                     glad::DataType::Float,
                     false,
                     sizeof(CellInstanceData),
                     reinterpret_cast<GLvoid*>(offsetof(CellInstanceData, position)))
            .divisor(1)
            .enable();

        glad::Unbind(*instance_array_buffer_);
        glad::Unbind(*vertex_array_);
    }

    void GridRenderer::init_shaders() {
        shader_program_ = std::make_unique<glad::Program>();

        auto vertex_shader = glad::VertexShader();
        vertex_shader.set_source(vertex_shader_source);

        auto fragment_shader = glad::FragmentShader();
        fragment_shader.set_source(fragment_shader_source);

        shader_program_->attach_shader(vertex_shader, fragment_shader);
        shader_program_->link();

        glad::Bind(*shader_program_);
        glad::UniformVec3(*shader_program_, "color").set(glm::value_ptr(live_cell_color_));
    }
}
