#pragma once

#include "application.h"
#include "engine.h"

namespace golxx {
    class Game {
    public:
        Game(Application& application, Engine& engine);

        ~Game();

        void run();

    private:
        Application& application_;
        Engine& engine_;
        glfw::Window& window_;
    };
}
