#pragma once
#include <memory>

#include "simulator.h"

#include "bw/engine/base_object.h"
#include "bw/engine/gl.h"

namespace golxx {
    class Background final : public bw::engine::BaseObject {
    public:
        explicit Background(const std::shared_ptr<Simulator>& simulator);

        ~Background() override = default;

        void init() override;

        void update(float deltaTime) override;

        void render(const bw::engine::Camera& camera) override;

    private:
        void init_mesh();
        void init_shaders();

        std::shared_ptr<Simulator> simulator_;
        int last_size_;

        std::unique_ptr<glad::VertexArray> vertex_array_;
        std::unique_ptr<glad::ArrayBuffer> array_buffer_;
        std::unique_ptr<glad::ElementArrayBuffer> element_array_buffer_;
        std::unique_ptr<glad::Program> shader_program_;
    };
}
