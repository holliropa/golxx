#include <fstream>
#include <sstream>
#include <iostream>
#include "files.hpp"

namespace utils {
    std::string readFile(const char *path) {
        std::string content;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            file.open(path);
            std::stringstream fileStream;

            fileStream << file.rdbuf();

            file.close();

            content = fileStream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cout << "ERROR::FILE::NOT_SUCCESSFULLY_READ: " << e.what() << '\n';
        }

        return content;
    }
}