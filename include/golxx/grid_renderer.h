#pragma once
#include <memory>

#include "camera.h"
#include "game_object.h"
#include "simulator.h"
#include "gl_common.h"

namespace golxx {
    class GridRenderer final : public GameObject {
    public:
        explicit GridRenderer(const std::shared_ptr<Simulator>& simulator,
                              const float cell_size,
                              const glm::vec3& live_cell_color)
            : simulator_(simulator),
              cell_size_(cell_size),
              live_cell_color_(live_cell_color) {}

        ~GridRenderer() override = default;

        void init() override;

        void update(float deltaTime) override;

        void render(const std::shared_ptr<Camera>& camera) override;

    private:
        void init_mesh();
        void init_shaders();

    private:
        std::unique_ptr<glad::VertexArray> vertex_array_;
        std::unique_ptr<glad::ArrayBuffer> array_buffer_;
        std::unique_ptr<glad::ArrayBuffer> instance_array_buffer_;
        std::unique_ptr<glad::ElementArrayBuffer> element_array_buffer_;
        std::unique_ptr<glad::Program> shader_program_;

        std::shared_ptr<Simulator> simulator_;


        float cell_size_;
        glm::vec3 live_cell_color_;
    };
}
