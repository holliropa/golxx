#include "game.h"

#include "grid_renderer.h"
#include "player.h"

namespace golxx {
    Game::Game(bw::engine::Application& application, bw::engine::Engine& engine)
        : bw::engine::Game(application, engine) {
        frame_buffer_event_handle_ = window_.framebufferSizeEvent.set(
            [this](const int width, const int height) {
                main_camera_->set_size({width, height});
            });

        window_.setSwapInterval(1);

        int w_width, w_height;
        window_.getWindowSize(&w_width, &w_height);

        simulator_ = std::make_shared<Simulator>();
        main_camera_ = std::make_shared<Camera>(20.0f, glm::vec2(w_width, w_height));
        camera_ = main_camera_;
        gameObjects_.emplace_back(std::make_shared<Player>(main_camera_, simulator_));
        gameObjects_.emplace_back(std::make_shared<GridRenderer>(simulator_));
    }

    Game::~Game() {
        window_.framebufferSizeEvent.unregister(frame_buffer_event_handle_);
    }
}
