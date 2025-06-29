#pragma once
#include <memory>

#include "simulator.h"
#include "bw/engine/game_object.h"
#include "bw/engine/gl.h"

namespace golxx {
    class GridRenderer final : public bw::engine::GameObject {
    public:
        explicit GridRenderer(const std::shared_ptr<Simulator>& simulator);

        ~GridRenderer() override = default;

        void init() override;

        void update(float deltaTime) override;

        void render(const bw::engine::Camera& camera) override;

    private:
        void init_mesh();
        void init_shaders();


        std::unique_ptr<glad::VertexArray> vertex_array_;
        std::unique_ptr<glad::ArrayBuffer> array_buffer_;
        std::unique_ptr<glad::ArrayBuffer> instance_array_buffer_;
        std::unique_ptr<glad::ElementArrayBuffer> element_array_buffer_;
        std::unique_ptr<glad::Program> shader_program_;

        std::shared_ptr<Simulator> simulator_;

        glm::vec3 live_cell_color_;
    };
}
