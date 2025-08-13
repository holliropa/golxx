#pragma once
#include <memory>

#include "camera.h"
#include "bw/engine/gl.h"
#include "simulator.h"

#include "bw/engine/base_object.h"

namespace golxx {
    class GridRenderer final : public bw::engine::BaseObject {
    public:
        explicit GridRenderer(const std::shared_ptr<Simulator>& simulator,
                              const glm::vec3& live_cell_color)
            : simulator_(simulator),
              live_cell_color_(live_cell_color) {}

        ~GridRenderer() override = default;

        void init() override;

        void update(float deltaTime) override;

        void render(const bw::engine::Camera& camera) override;

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

        glm::vec3 live_cell_color_;
    };
}
