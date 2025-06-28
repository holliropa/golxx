#pragma once
#include <memory>

#include "simulator.h"
#include "bw/engine/game_object.h"

namespace golxx {
    class Player final : public bw::engine::GameObject {
    public:
        explicit Player(const std::shared_ptr<bw::engine::Camera>& camera,
                        const std::shared_ptr<Simulator>& simulator);
        ~Player() override = default;

        void update(float deltaTime) override;

    private:
        void toggle_line_cells(glm::ivec2 from, glm::ivec2 to, bool toggle);

    private:
        std::shared_ptr<bw::engine::Camera> camera_;
        std::shared_ptr<Simulator> simulator_;

        float speed_;

        bool is_drawing_line_ = false;
        bool drawing_state_ = false;
        glm::ivec2 last_cell_{};
    };
}
