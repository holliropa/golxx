#include "golxx/game.h"

#include <iostream>
#include <unordered_set>

#include "golxx/application.h"
#include "golxx/engine.h"
#include "golxx/input.h"

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

namespace golxx {
    struct Camera {
        glm::vec3 position{};
        glm::mat4 projection{};
        float zoomLevel = 20.0f;
    };

    struct ViewBounds {
        float left;
        float right;
        float bottom;
        float top;
    };

    struct CellVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct CellInstanceData {
        glm::vec2 position;
    };

    auto live_cell_color = glm::vec3(1.0f, 1.0f, 1.0f);

    std::unordered_set<glm::ivec2> live_cells;
    glm::ivec2 last_cell;
    glm::ivec2 window_size{};

    std::unique_ptr<glad::VertexArray> vertex_array;
    std::unique_ptr<glad::ArrayBuffer> array_buffer;
    std::unique_ptr<glad::ArrayBuffer> instance_array_buffer;
    std::unique_ptr<glad::ElementArrayBuffer> element_array_buffer;
    std::unique_ptr<glad::Program> shader_program;
    Camera camera;
    float cell_size = 1.0f;
    float move_speed = 15.0f;

    void update_projection() {
        const auto aspectRatio =
            static_cast<float>(window_size.x) / static_cast<float>(window_size.y);

        camera.projection = glm::ortho(
            -1.0f * aspectRatio * camera.zoomLevel,
            1.0f * aspectRatio * camera.zoomLevel,
            -camera.zoomLevel,
            camera.zoomLevel
        );
    }

    void set_window_size(const int width, const int height) {
        window_size = {width, height};
        update_projection();
    }

    void init_mesh() {
        vertex_array = std::make_unique<glad::VertexArray>();
        array_buffer = std::make_unique<glad::ArrayBuffer>();
        instance_array_buffer = std::make_unique<glad::ArrayBuffer>();
        element_array_buffer = std::make_unique<glad::ElementArrayBuffer>();

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

        glad::Bind(*vertex_array);

        array_buffer->data(sizeof(CellVertex) * cell_vertices.size(), cell_vertices.data());
        element_array_buffer->data(sizeof(GLuint) * indices.size(), indices.data());

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

        glad::Bind(*instance_array_buffer);
        glad::VertexAttribute(3)
            .pointer(2,
                     glad::DataType::Float,
                     false,
                     sizeof(CellInstanceData),
                     reinterpret_cast<GLvoid*>(offsetof(CellInstanceData, position)))
            .divisor(1)
            .enable();

        glad::Unbind(*instance_array_buffer);
        glad::Unbind(*vertex_array);
    }

    void init_shaders() {
        shader_program = std::make_unique<glad::Program>();

        auto vertex_shader = glad::VertexShader();
        vertex_shader.set_source(vertex_shader_source);

        auto fragment_shader = glad::FragmentShader();
        fragment_shader.set_source(fragment_shader_source);

        shader_program->attach_shader(vertex_shader, fragment_shader);
        shader_program->link();

        glad::Bind(*shader_program);
        glad::UniformVec3(*shader_program, "color").set(glm::value_ptr(live_cell_color));
    }

    ViewBounds get_camera_bounds(const Camera& camera, const float aspectRatio) {
        const float halfWidth = aspectRatio * camera.zoomLevel;
        const float halfHeight = camera.zoomLevel;

        return {
            .left = camera.position.x - halfWidth,
            .right = camera.position.x + halfWidth,
            .bottom = camera.position.y - halfHeight,
            .top = camera.position.y + halfHeight,
        };
    }

    std::unordered_set<glm::ivec2> do_cycle(const std::unordered_set<glm::ivec2>& cells) {
        std::unordered_set<glm::ivec2> nextCells{};
        std::unordered_set<glm::ivec2> activeCells{};

        const std::vector<glm::ivec2> neighborOffsets = {
            {-1, -1},
            {-1, 0},
            {-1, 1},
            {0, -1},
            {0, 1},
            {1, -1},
            {1, 0},
            {1, 1}
        };

        for (const auto& cell : cells) {
            activeCells.insert(cell);

            for (const auto& offset : neighborOffsets) {
                activeCells.insert(cell + offset);
            }
        }

        for (const auto& cell : activeCells) {
            int liveNeighbors = 0;

            for (const auto& offset : neighborOffsets) {
                if (cells.find(cell + offset) != cells.end()) {
                    liveNeighbors++;
                }
            }

            if (cells.find(cell) != cells.end()) {
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    nextCells.erase(cell); // Cell dies
                }
                else {
                    nextCells.insert(cell); // Cell lives
                }
            }
            else {
                if (liveNeighbors == 3) {
                    nextCells.insert(cell); // Cell becomes alive
                }
            }
        }

        return nextCells;
    }

    glm::ivec2 get_movement_offset() {
        glm::ivec2 offset{};

        if (Input::GetKeyPressed(glfw::KeyCode::W)) {
            offset.y++;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::S)) {
            offset.y--;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::A)) {
            offset.x--;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::D)) {
            offset.x++;
        }

        return offset;
    }

    Game::Game(Application& application, Engine& engine)
        : application_(application),
          engine_(engine),
          window_(application_.getWindow()) {
        window_.keyEvent.set(
            [](const glfw::KeyCode key, int, const glfw::KeyAction action, glfw::ModifierKeyBit) {
                Input::HandleKey(key, action);
            });
        window_.mouseButtonEvent.set(
            [](const glfw::MouseButton mouse_button, const glfw::MouseButtonAction action, glfw::ModifierKeyBit) {
                Input::HandleMouseButton(mouse_button, action);
            });
        window_.framebufferSizeEvent.set(
            [this](const int width, const int height) {
                glad::Viewport(width, height);

                set_window_size(width, height);
            });
        window_.cursorPosEvent.set(
            [](const double x, const double y) {
                Input::HandleCursorPosition(static_cast<float>(x), static_cast<float>(y));
            });
        window_.scrollEvent.set(
            [](const double x, const double y) {
                Input::HandleScroll(static_cast<float>(x), static_cast<float>(y));
            });

        // window_.setCursorMode(glfw::CursorMode::Disabled);
        int w_width, w_height;
        window_.getWindowSize(&w_width, &w_height);
        window_.setCursorPosition(static_cast<double>(w_width) / 2,
                                  static_cast<double>(w_height) / 2);
        set_window_size(w_width, w_height);

        glad::Enable(glad::Capability::DepthTest);
        glad::Enable(glad::Capability::CullFace);

        init_mesh();
        init_shaders();
    }

    Game::~Game() {
        vertex_array.reset();
        array_buffer.reset();
        instance_array_buffer.reset();
        element_array_buffer.reset();
        shader_program.reset();
    }

    void Game::run() {
        auto last_time = glfw::getTime();
        auto last_FPS_update_time = glfw::getTime();
        unsigned frameCount = 0;

        while (!window_.shouldClose()) {
            glfwPollEvents();
            Input::Update();

            const auto current_time = glfw::getTime();
            const auto delta = static_cast<float>(current_time - last_time);
            last_time = current_time;

            frameCount++;
            const auto time_since_FPS_update = current_time - last_FPS_update_time;
            if (time_since_FPS_update > 1.0) {
                const auto fps = static_cast<double>(frameCount) / time_since_FPS_update;
                std::cout << "FPS: " << fps << '\n';
                last_FPS_update_time = current_time;
                frameCount = 0;
            }
            /*
             * UPDATE
             */

            if (const auto& scroll = Input::GetScrollOffset(); scroll.y != 0.0f) {
                const int scrollOffset = scroll.y * std::max(static_cast<int>(0.1f * camera.zoomLevel), 1);;
                camera.zoomLevel = std::max(camera.zoomLevel - scrollOffset, 1.0f);
                update_projection();
            }

            if (const auto& movement = get_movement_offset(); movement != glm::zero<glm::ivec2>()) {
                camera.position += glm::vec3(movement.x, movement.y, 0.0f) * cell_size * delta * move_speed;
            }

            if (Input::GetKeyPressed(glfw::KeyCode::Space) || Input::GetKeyDown(glfw::KeyCode::LeftShift)) {
                live_cells = do_cycle(live_cells);
            }

            if (Input::GetMouseButtonPressed(glfw::MouseButton::Left)) {
                const auto cursor = Input::GetCursorPosition();

                glm::vec4 ndcPos(
                    2.0f * cursor.x / static_cast<float>(window_size.x) - 1.0f,
                    1.0f - 2.0f * cursor.y / static_cast<float>(window_size.y),
                    0.0f,
                    1.0f);

                glm::mat4 inverseProjView = glm::inverse(
                    camera.projection * glm::translate(glm::identity<glm::mat4>(), -camera.position));
                glm::vec4 worldPos = inverseProjView * ndcPos;

                worldPos /= worldPos.w;

                const auto column = static_cast<int>(std::floor(worldPos.x));
                const auto row = static_cast<int>(std::floor(worldPos.y));
                auto cellPosition = glm::ivec2(column, row);

                if (cellPosition != last_cell) {
                    last_cell = cellPosition;
                    if (live_cells.find(cellPosition) != live_cells.end()) {
                        live_cells.erase(cellPosition);
                    }
                    else {
                        live_cells.insert(cellPosition);
                    }
                }
            }

            /*
             * RENDER
             */

            glad::ClearBuffers().Color().Depth();
            glad::ClearColor(0.1f, 0.4f, 0.7f);

            glad::Bind(*shader_program);
            glad::UniformMat4(*shader_program, "projection").set(glm::value_ptr(camera.projection));
            const auto view = glm::translate(glm::identity<glm::mat4>(), -camera.position);
            glad::UniformMat4(*shader_program, "view").set(glm::value_ptr(view));
            auto model = glm::identity<glm::mat4>();
            model = glm::scale(model, glm::vec3(cell_size, cell_size, 1.0f));
            model = glm::translate(model, glm::vec3(0.5f));
            glad::UniformMat4(*shader_program, "model").set(glm::value_ptr(model));

            std::vector<CellInstanceData> cells(live_cells.size());
            unsigned i = 0;
            for (const auto& liveCell : live_cells) {
                cells[i++] = {
                    .position = {
                        static_cast<float>(liveCell.x) * cell_size,
                        static_cast<float>(liveCell.y) * cell_size,
                    },
                };
            }

            instance_array_buffer->data(
                sizeof(CellInstanceData) * cells.size(),
                cells.data(),
                glad::BufferUsage::DynamicDraw
            );

            glad::Bind(*vertex_array);
            glad::DrawElementsInstanced(
                glad::PrimitiveType::Triangles,
                6,
                glad::IndexType::UnsignedInt,
                cells.size()
            );

            if (Input::GetKeyDown(glfw::KeyCode::Escape)) {
                window_.setShouldClose(true);
            }

            window_.swapBuffers();
        }
    }
}
