#include "background.h"

namespace golxx {
    auto background_vertex_shader_source = R"(
#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 position = a_position;

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

    Background::Background(const std::shared_ptr<Simulator>& simulator)
        : simulator_(simulator), last_size_(0) {}

    void Background::init() {
        init_mesh();
        init_shaders();
    }

    void Background::update(float deltaTime) {
        const auto root = simulator_->get_root();
        if (root && root->level != last_size_) {
            last_size_ = root->level;
        }
    }


    void Background::render(const bw::engine::Camera& camera) {
        glad::Enable(glad::Capability::Blend);
        glad::BlendFunc(glad::BlendFunction::SrcAlpha, glad::BlendFunction::OneMinusSrcAlpha);

        const auto line_width_lock = glad::TemporaryLineWidth(0.1f);

        glad::Bind(*shader_program_);
        glad::Bind(*vertex_array_);

        glad::UniformMat4(*shader_program_, "projection").set(glm::value_ptr(camera.get_projection()));
        glad::UniformMat4(*shader_program_, "view").set(glm::value_ptr(camera.get_view()));

        const auto size = std::pow(2, last_size_);

        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(0.0f, size / 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(std::pow(2, last_size_), 1.0f, 1.0f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));

        glad::DrawElements(
            glad::PrimitiveType::Lines,
            2,
            glad::IndexType::UnsignedInt
        );


        model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(0.0f, -size / 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(std::pow(2, last_size_), 1.0f, 1.0f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));


        glad::DrawElements(
            glad::PrimitiveType::Lines,
            2,
            glad::IndexType::UnsignedInt
        );


        model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(size / 2.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(std::pow(2, last_size_), 1.0f, 1.0f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));


        glad::DrawElements(
            glad::PrimitiveType::Lines,
            2,
            glad::IndexType::UnsignedInt
        );


        model = glm::identity<glm::mat4>();
        model = glm::translate(model, glm::vec3(-size / 2.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(std::pow(2, last_size_), 1.0f, 1.0f));
        glad::UniformMat4(*shader_program_, "model").set(glm::value_ptr(model));


        glad::DrawElements(
            glad::PrimitiveType::Lines,
            2,
            glad::IndexType::UnsignedInt
        );


        glad::Disable(glad::Capability::Blend);
    }

    void Background::init_mesh() {
        vertex_array_ = std::make_unique<glad::VertexArray>();
        array_buffer_ = std::make_unique<glad::ArrayBuffer>();
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
