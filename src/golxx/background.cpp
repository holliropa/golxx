#include "golxx/background.h"

#include "golxx/input.h"

namespace golxx {
    auto background_vertex_shader_source = R"(
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_instance_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 position = a_position + vec3(a_instance_position, 0.0);

    gl_Position = projection * view * model * vec4(position, 1.0);
})";

    auto background_fragment_shader_source = R"(
#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(vec3(1.0), 1.0);
})";

    struct LineVertex {
        glm::vec3 position;
    };

    struct LineInstanceData {
        glm::vec2 position;
    };

    void Background::init() {
        init_mesh();
        init_shaders();
    }

    void Background::render(const std::shared_ptr<Camera>& camera) {
        glad::Enable(glad::Capability::Blend);
        glad::BlendFunc(glad::BlendFunction::SrcAlpha, glad::BlendFunction::OneMinusSrcAlpha);

        const auto line_width_lock = glad::TemporaryLineWidth(0.1f);

        glad::Bind(*shader_program_);
        glad::Bind(*vertex_array_);

        glad::UniformMat4(*shader_program_, "projection").set(glm::value_ptr(camera->get_projection()));
        const auto view = glm::translate(glm::identity<glm::mat4>(), -camera->position);
        glad::UniformMat4(*shader_program_, "view").set(glm::value_ptr(view));
        auto model = glm::identity<glm::mat4>();
        model = glm::scale(model, glm::vec3(1000.0f, 1.0f, 1.0f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));


        glad::DrawElementsInstanced(
            glad::PrimitiveType::Lines,
            2,
            glad::IndexType::UnsignedInt,
            static_cast<GLsizei>(instance_count_)
        );

        model = glm::identity<glm::mat4>();
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1000.0f, 1.0f, 1.0f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));

        glad::DrawElementsInstanced(
            glad::PrimitiveType::Lines,
            2,
            glad::IndexType::UnsignedInt,
            static_cast<GLsizei>(instance_count_)
        );

        glad::Disable(glad::Capability::Blend);
    }

    void Background::init_mesh() {
        vertex_array_ = std::make_unique<glad::VertexArray>();
        array_buffer_ = std::make_unique<glad::ArrayBuffer>();
        instance_array_buffer_ = std::make_unique<glad::ArrayBuffer>();
        element_array_buffer_ = std::make_unique<glad::ElementArrayBuffer>();

        const std::vector<LineVertex> vertices = {
            {{-0.5f, 0.0f, 0.0f}},
            {{0.5f, 0.0f, 0.0f}},
        };

        const std::vector<GLuint> indices = {
            0, 1
        };

        glad::Bind(*vertex_array_);

        array_buffer_->data(sizeof(LineVertex) * vertices.size(), vertices.data());
        element_array_buffer_->data(sizeof(GLuint) * indices.size(), indices.data());

        glad::VertexAttribute(0)
            .pointer(3,
                     glad::DataType::Float,
                     false,
                     sizeof(LineVertex),
                     reinterpret_cast<GLvoid*>(offsetof(LineVertex, position)))
            .enable();

        glad::Bind(*instance_array_buffer_);
        glad::VertexAttribute(1)
            .pointer(2,
                     glad::DataType::Float,
                     false,
                     sizeof(LineInstanceData),
                     reinterpret_cast<GLvoid*>(offsetof(LineInstanceData, position)))
            .divisor(1)
            .enable();

        std::vector<LineInstanceData> lines;
        for (int delta = -100; delta <= 100; delta += 1)
            lines.emplace_back(LineInstanceData{{0.0f, delta}});
        instance_array_buffer_->data(
            sizeof(LineInstanceData) * lines.size(),
            lines.data(),
            glad::BufferUsage::DynamicDraw
        );

        instance_count_ = lines.size();
    }

    void Background::init_shaders() {
        shader_program_ = std::make_unique<glad::Program>();

        auto vertex_shader = glad::VertexShader();
        vertex_shader.set_source(background_vertex_shader_source);

        auto fragment_shader = glad::FragmentShader();
        fragment_shader.set_source(background_fragment_shader_source);

        shader_program_->attach_shader(vertex_shader, fragment_shader);
        shader_program_->link();
    }
}
