#pragma once

#include "application.h"
#include "camera.h"
#include "engine.h"
#include "game_object.h"
#include "simulator.h"

namespace golxx {
    class Game {
    public:
        Game(Application& application, Engine& engine);

        ~Game();

        void run();

    private:
        void update(float deltaTime);
        void render();

    private:
        Application& application_;
        Engine& engine_;
        glfw::Window& window_;

        std::shared_ptr<Simulator> simulator_;
        std::shared_ptr<Camera> camera_;
        std::vector<std::shared_ptr<GameObject>> gameObjects_;
    };
}
