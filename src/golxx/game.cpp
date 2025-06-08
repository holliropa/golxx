#include "golxx/game.h"

#include <iostream>
#include "golxx/application.h"
#include "golxx/camera.h"
#include "golxx/config_manager.h"
#include "golxx/engine.h"
#include "golxx/fps_counter.h"
#include "golxx/game_object.h"
#include "golxx/grid_renderer.h"
#include "golxx/input.h"
#include "golxx/player.h"
#include "golxx/simulator.h"
#include "golxx/time_manager.h"


namespace golxx {
    Game::Game(Application& application, Engine& engine)
        : application_(application),
          engine_(engine),
          window_(application_.getWindow()) {
        window_.keyEvent.set(
            [](const glfw::KeyCode key, int, const glfw::KeyAction action, glfw::ModifierKeyBit) {
                Input::HandleKey(key, action);
            });
        window_.mouseButtonEvent.set(
            [](const glfw::MouseButton mouse_button, const glfw::MouseButtonAction action, glfw::ModifierKeyBit) {
                Input::HandleMouseButton(mouse_button, action);
            });
        window_.framebufferSizeEvent.set(
            [this](const int width, const int height) {
                glad::Viewport(width, height);
                camera_->set_size({width, height});
            });
        window_.cursorPosEvent.set(
            [](const double x, const double y) {
                Input::HandleCursorPosition(static_cast<float>(x), static_cast<float>(y));
            });
        window_.scrollEvent.set(
            [](const double x, const double y) {
                Input::HandleScroll(static_cast<float>(x), static_cast<float>(y));
            });

        window_.setSwapInterval(1);

        ConfigManager::get().loadFromFile("config.json");
        const auto& config = ConfigManager::get().getConfig();

        int w_width, w_height;
        window_.getWindowSize(&w_width, &w_height);

        simulator_ = std::make_shared<Simulator>();
        camera_ = std::make_shared<Camera>(20.0f, glm::vec2(w_width, w_height));
        gameObjects_.emplace_back(std::make_shared<Player>(camera_, simulator_, config.playerSpeed));
        gameObjects_.emplace_back(std::make_shared<GridRenderer>(
            simulator_,
            config.liveCellColor));

        camera_->set_size({w_width, w_height});

        for (const auto& gameObject : gameObjects_) {
            gameObject->init();
        }
    }

    Game::~Game() = default;

    void Game::run() {
        TimeManager timeManager;
        FPSCounter fpsCounter;

        while (!window_.shouldClose()) {
            glfwPollEvents();
            Input::Update();

            const auto deltaTime = timeManager.getDeltaTime();
            fpsCounter.update();

            update(deltaTime);
            render();

            if (Input::GetKeyDown(glfw::KeyCode::Escape)) {
                window_.setShouldClose(true);
            }

            window_.swapBuffers();
        }
    }

    void Game::update(const float deltaTime) {
        for (const auto& gameObject : gameObjects_) {
            gameObject->update(deltaTime);
        }

        if (Input::GetKeyPressed(glfw::KeyCode::Space) || Input::GetKeyDown(glfw::KeyCode::LeftShift)) {
            simulator_->run_cycle();
        }
    }

    void Game::render() {
        const auto& config = ConfigManager::get().getConfig();

        glad::ClearBuffers().Color().Depth();
        const auto& bgColor = config.backgroundColor;
        glad::ClearColor(bgColor.r, bgColor.g, bgColor.b);

        for (const auto& gameObject : gameObjects_) {
            gameObject->render(camera_);
        }
    }
}
