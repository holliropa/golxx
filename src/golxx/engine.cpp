#include "golxx/engine.h"

namespace golxx {
    Engine::Engine() {
        if (!glad::load()) {
            throw std::runtime_error("GLAD Load Failed");
        }
    }

    Engine::~Engine() {
        glad::unload();
    }
}
