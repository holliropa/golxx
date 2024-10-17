#pragma once

#include <map>
#include "glm_common.hpp"
#include "key_code.hpp"
#include "button_code.hpp"

class Game {
public:
    bool IsExit = false;
    unsigned int Width, Height;

    glm::vec2 CursorPosition;
    bool IsMouseInside;
    std::map<glfwxx::ButtonCode, bool> DownButtons;
    std::map<glfwxx::ButtonCode, bool> UpButtons;
    std::map<glfwxx::ButtonCode, bool> PressedButtons;
    std::map<glfwxx::KeyCode, bool> DownKeys;
    std::map<glfwxx::KeyCode, bool> UpKeys;
    std::map<glfwxx::KeyCode, bool> PressedKeys;

    Game(unsigned int width, unsigned int height);

    ~Game();

    void Initialize();

    void Resize(unsigned int width, unsigned int height);

    void Update(float deltaTime);

    void Render();

};