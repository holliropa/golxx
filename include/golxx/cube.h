#pragma once
#include <memory>

#include "bw/engine/base_object.h"
#include "bw/engine/gl.h"

namespace golxx {
    class Cube final : public bw::engine::BaseObject {
    public:
        explicit Cube() = default;

        ~Cube() override = default;

        void init() override;

        void render(const bw::engine::Camera& camera) override;

    private:
        void init_mesh();
        void init_shaders();

    private:
        std::unique_ptr<glad::VertexArray> vertex_array_;
        std::unique_ptr<glad::ArrayBuffer> array_buffer_;
        std::unique_ptr<glad::ElementArrayBuffer> element_array_buffer_;
        std::unique_ptr<glad::Program> shader_program_;
    };
}
