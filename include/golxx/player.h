#pragma once
#include <memory>

#include "camera.h"
#include "game_object.h"
#include "simulator.h"

namespace golxx {
    class Player final : public GameObject {
    public:
        float speed = 1.0f;

        explicit Player(const std::shared_ptr<Camera>& camera,
                        const std::shared_ptr<Simulator>& simulator);
        ~Player() override = default;

        void update(float deltaTime) override;

    private:
        void toggle_line_cells(glm::ivec2 from, glm::ivec2 to, bool toggle);

    private:
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<Simulator> simulator_;

        bool is_drawing_line_ = false;
        bool drawing_state_ = false;
        glm::ivec2 last_cell_{};
    };
}
