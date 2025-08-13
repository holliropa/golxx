#pragma once

#include "camera.h"
#include "simulator.h"

#include "bw/engine/application.h"
#include "bw/engine/engine.h"
#include "bw/engine/game.h"

namespace golxx {
    class GolxxGame final : public bw::engine::Game {
    public:
        GolxxGame(bw::engine::Application& application, bw::engine::Engine& engine);

        ~GolxxGame();

    private:
        std::shared_ptr<Simulator> simulator_;
        std::shared_ptr<Camera> main_camera_;
    };
}
