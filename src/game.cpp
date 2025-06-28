#include "game.h"

#include "grid_renderer.h"
#include "player.h"

namespace golxx {
    Game::Game(bw::engine::Application& application, bw::engine::Engine& engine)
        : bw::engine::Game(application, engine) {
        window_.setSwapInterval(1);

        int w_width, w_height;
        window_.getWindowSize(&w_width, &w_height);

        simulator_ = std::make_shared<Simulator>();
        camera_ = std::make_shared<bw::engine::Camera>(20.0f, glm::vec2(w_width, w_height));
        gameObjects_.emplace_back(std::make_shared<Player>(camera_, simulator_));
        gameObjects_.emplace_back(std::make_shared<GridRenderer>(simulator_));

        camera_->set_size({w_width, w_height});
    }

    Game::~Game() = default;
}
