#include <iostream>
#include <chrono>
#include "application.hpp"

#include "game.hpp"
#include "button_code.hpp"

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void mouse_position_callback(GLFWwindow *window, double x, double y);

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void cursor_enter_callback(GLFWwindow *window, int entered);

void process_inputs();

// The Width of the screen
const unsigned int SCREEN_WIDTH = 600;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

GLFWwindow *pWindow;
std::string title;
Game *pGame;

float lastTime = 0.0f;          // Time of the last frame
float lastDelta = 1.0f / 60.0f; // Smoothed delta time
int frameCount = 0;             // Number of frames since the last title update
float fpsTime = 0.0f;           // Time accumulator for FPS calculation
float startTime = 0.0f;         // Record the start time

Application::Application() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, false);

    title = "Game of Life";
    pWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title.c_str(), nullptr, nullptr);
    if (pWindow == nullptr) {
        std::cout << "Failed to create GLFW m_window" << '\n';
        glfwTerminate();
        throw std::runtime_error("Window Creation Failed");
    }

    glfwMakeContextCurrent(pWindow);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoaderLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("GLAD Initialization Failed");
    }

    glfwSetKeyCallback(pWindow, key_callback);
    glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(pWindow, mouse_position_callback);
    glfwSetMouseButtonCallback(pWindow, mouse_button_callback);
    glfwSetCursorEnterCallback(pWindow, cursor_enter_callback);

    // initialize game
    // ---------------
    pGame = new Game;
    pGame->Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
    pGame->Initialize();

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Application::~Application() {
    if (pGame) {
        delete pGame;
        pGame = nullptr;
    }


    if (pWindow) {
        glfwDestroyWindow(pWindow);
        pWindow = nullptr;
    }

    glfwTerminate();
}

void Application::run() {
    startTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(pWindow)) {
        auto frameStartTime = std::chrono::high_resolution_clock::now(); // Start time of the frame

        auto currentTime = static_cast<float>(glfwGetTime());
        auto deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Smooth the delta time
        lastDelta = (lastDelta + deltaTime) / 2;

        // Accumulate time for FPS counter
        fpsTime += deltaTime;
        frameCount++;

        // Update the window title every 1 second (or any desired interval)
        if (fpsTime >= 1.0f) {
            float fps = static_cast<float>(frameCount) / fpsTime;  // Calculate FPS
            float elapsedTime = currentTime - startTime;  // Calculate time since start

            // Update the window title with both FPS and elapsed time
            std::string current_title = title + " | FPS: " + std::to_string(static_cast<int>(fps))
                                        + " | Time: " + std::to_string(static_cast<int>(elapsedTime)) + "s"
                                        + " | Cells: " + std::to_string(pGame->GetLiveCellsCount());
            glfwSetWindowTitle(pWindow, current_title.c_str());

            // Reset counters
            fpsTime = 0.0f;
            frameCount = 0;
        }

        process_inputs();

        pGame->Update(deltaTime);

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        pGame->Render();

        glfwSwapBuffers(pWindow);

        if (pGame->IsExit) {
            glfwSetWindowShouldClose(pWindow, true);
        }
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    auto keyCode = static_cast<glfwxx::KeyCode>(key);

    if (action == GLFW_PRESS) {
        pGame->PressedKeys[keyCode] = true;
        pGame->DownKeys[keyCode] = true;
    } else if (action == GLFW_RELEASE) {
        pGame->PressedKeys[keyCode] = false;
        pGame->UpKeys[keyCode] = true;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    pGame->Resize(width, height);
}

void mouse_position_callback(GLFWwindow *window, double x, double y) {
    pGame->CursorPosition.x = static_cast<float>(x);
    pGame->CursorPosition.y = static_cast<float>(y);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    auto buttonCode = static_cast<glfwxx::ButtonCode>(button);

    if (action == GLFW_PRESS) {
        pGame->PressedButtons[buttonCode] = true;
        pGame->DownButtons[buttonCode] = true;
    } else if (action == GLFW_RELEASE) {
        pGame->PressedButtons[buttonCode] = false;
        pGame->UpButtons[buttonCode] = true;
    }
}

void cursor_enter_callback(GLFWwindow *window, int entered) {
    pGame->IsMouseInside = entered;
}

void process_inputs() {
    for (auto &downKeyPair: pGame->DownKeys) {
        downKeyPair.second = false;
    }

    for (auto &upKeyPair: pGame->UpKeys) {
        upKeyPair.second = false;
    }

    for (auto &downButtonPair: pGame->DownButtons) {
        downButtonPair.second = false;
    }

    for (auto &upButtonPair: pGame->UpButtons) {
        upButtonPair.second = false;
    }

    glfwPollEvents();
}