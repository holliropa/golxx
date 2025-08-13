#include "golxx/golxx_game.h"

#include "bw/engine/input.h"

#include "golxx/background.h"
#include "golxx/camera.h"
#include "golxx/config_manager.h"
#include "golxx/cube.h"
#include "golxx/grid_renderer.h"
#include "golxx/player.h"
#include "game-of-life/hashlife.h"


namespace golxx {
    GolxxGame::GolxxGame(bw::engine::Application& application, bw::engine::Engine& engine)
        : Game(application, engine) {
        window_.setSwapInterval(1);

        ConfigManager::get().loadFromFile("config.json");
        const auto& config = ConfigManager::get().getConfig();

        int w_width, w_height;
        window_.getWindowSize(&w_width, &w_height);

        simulator_ = std::make_shared<Simulator>();
        camera_ = main_camera_ = std::make_shared<Camera>(20.0f, glm::vec2(w_width, w_height));
        gameObjects_.emplace_back(std::make_shared<Player>(main_camera_, simulator_, config.playerSpeed));
        gameObjects_.emplace_back(std::make_shared<Cube>());
        gameObjects_.emplace_back(std::make_shared<Background>(simulator_));
        gameObjects_.emplace_back(std::make_shared<GridRenderer>(simulator_, config.liveCellColor));

        main_camera_->set_size({w_width, w_height});

        for (const auto& gameObject : gameObjects_) {
            gameObject->init();
        }
    }

    GolxxGame::~GolxxGame() = default;
}
