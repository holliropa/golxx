#pragma once

#include "simulator.h"
#include "bw/engine/application.h"
#include "bw/engine/engine.h"
#include "bw/engine/game.h"

namespace golxx {
    class Game : public bw::engine::Game {
    public:
        Game(bw::engine::Application& application, bw::engine::Engine& engine);

        ~Game();

    private:
        std::shared_ptr<Simulator> simulator_;
    };
}
