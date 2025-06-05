#include <iostream>

#include "golxx/application.h"
#include "golxx/engine.h"
#include "golxx/game.h"

constexpr unsigned int WIDTH = 800;
constexpr unsigned int HEIGHT = 800;

int main() {
    try {
        golxx::Application application(WIDTH, HEIGHT, "Golxx");
        golxx::Engine engine;
        golxx::Game game(application, engine);

        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }


    return 0;
}
