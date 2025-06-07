#include "golxx/game.h"

#include <iostream>
#include "golxx/application.h"
#include "golxx/camera.h"
#include "golxx/engine.h"
#include "golxx/game_object.h"
#include "golxx/grid_renderer.h"
#include "golxx/input.h"
#include "golxx/player.h"
#include "golxx/simulator.h"


namespace golxx {
    struct ViewBounds {
        float left;
        float right;
        float bottom;
        float top;
    };

    auto live_cell_color = glm::vec3(1.0f, 1.0f, 1.0f);
    float cell_size = 1.0f;

    glm::ivec2 window_size{};

    std::shared_ptr<Simulator> simulator;
    std::shared_ptr<Camera> camera;
    std::vector<std::shared_ptr<GameObject>> gameObjects;

    void set_window_size(const int width, const int height) {
        window_size = {width, height};
        camera->set_size({width, height});
    }

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

                set_window_size(width, height);
            });
        window_.cursorPosEvent.set(
            [](const double x, const double y) {
                Input::HandleCursorPosition(static_cast<float>(x), static_cast<float>(y));
            });
        window_.scrollEvent.set(
            [](const double x, const double y) {
                Input::HandleScroll(static_cast<float>(x), static_cast<float>(y));
            });

        int w_width, w_height;
        window_.getWindowSize(&w_width, &w_height);
        window_.setCursorPosition(static_cast<double>(w_width) / 2,
                                  static_cast<double>(w_height) / 2);

        simulator = std::make_shared<Simulator>();
        camera = std::make_shared<Camera>(20.0f, glm::vec2(w_width, w_height));
        gameObjects.emplace_back(std::make_shared<Player>(camera, simulator));
        gameObjects.emplace_back(std::make_shared<GridRenderer>(simulator, cell_size, live_cell_color));

        set_window_size(w_width, w_height);

        for (const auto& gameObject : gameObjects) {
            gameObject->init();
        }
    }

    Game::~Game() {
        gameObjects.clear();
        simulator.reset();
        camera.reset();
    }

    void Game::run() {
        auto last_time = glfw::getTime();
        auto last_FPS_update_time = glfw::getTime();
        unsigned frameCount = 0;

        while (!window_.shouldClose()) {
            glfwPollEvents();
            Input::Update();

            const auto current_time = glfw::getTime();
            const auto delta = static_cast<float>(current_time - last_time);
            last_time = current_time;

            frameCount++;
            const auto time_since_FPS_update = current_time - last_FPS_update_time;
            if (time_since_FPS_update > 1.0) {
                const auto fps = static_cast<double>(frameCount) / time_since_FPS_update;
                std::cout << "FPS: " << fps << '\n';
                last_FPS_update_time = current_time;
                frameCount = 0;
            }
            /*
             * UPDATE
             */

            for (const auto& gameObject : gameObjects) {
                gameObject->update(delta);
            }

            if (Input::GetKeyPressed(glfw::KeyCode::Space) || Input::GetKeyDown(glfw::KeyCode::LeftShift)) {
                simulator->run_cycle();
            }

            /*
             * RENDER
             */

            glad::ClearBuffers().Color().Depth();
            glad::ClearColor(0.1f, 0.4f, 0.7f);

            for (const auto& gameObject : gameObjects) {
                gameObject->render(camera);
            }

            if (Input::GetKeyDown(glfw::KeyCode::Escape)) {
                window_.setShouldClose(true);
            }

            window_.swapBuffers();
        }
    }
}
