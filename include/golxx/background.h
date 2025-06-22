#pragma once
#include <memory>

#include "camera.h"
#include "game_object.h"
#include "gl_common.h"
#include "simulator.h"

namespace golxx {
    class Background final : public GameObject {
    public:
        explicit Background(const std::shared_ptr<Simulator>& simulator);

        ~Background() override = default;

        void init() override;

        void update(float deltaTime) override;

        void render(const std::shared_ptr<Camera>& camera) override;

    private:
        void init_mesh();
        void init_shaders();

    private:
        std::shared_ptr<Simulator> simulator_;
        int last_size_;

        std::unique_ptr<glad::VertexArray> vertex_array_;
        std::unique_ptr<glad::ArrayBuffer> array_buffer_;
        std::unique_ptr<glad::ElementArrayBuffer> element_array_buffer_;
        std::unique_ptr<glad::Program> shader_program_;
    };
}
