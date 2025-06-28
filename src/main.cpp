#include <iostream>

#include "bw/engine/application.h"
#include "bw/engine/engine.h"
#include "game.h"

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 800;

int main() {
    try {
        bw::engine::Application application(WIDTH, HEIGHT, "Game of Life");
        bw::engine::Engine engine;
        golxx::Game game(application, engine);

        game.init();

        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }


    return 0;
}
