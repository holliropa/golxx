#pragma once
#include <memory>
#include "gl_common.h"

namespace golxx {
    class Application {
    public:
        Application(int width, int height, const char* title);
        ~Application();

        [[nodiscard]] glfw::Window& getWindow() const {
            return *pWindow_;
        }

    private:
        std::unique_ptr<glfw::Window> pWindow_;
    };
}
