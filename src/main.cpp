#include <iostream>

#include "golxx/golxx_game.h"

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 800;

int main() {
    try {
        bw::engine::Application application(WIDTH, HEIGHT, "Golxx");
        bw::engine::Engine engine;
        golxx::GolxxGame game(application, engine);

        game.init();

        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }


    return 0;
}
