#include "golxx/input.h"

namespace golxx {
    typedef struct {
        glfw::KeyCode code;
        glfw::KeyAction action;
    } UnhandledKey;

    typedef struct {
        glfw::MouseButton mouse_button;
        glfw::MouseButtonAction action;
    } UnhandledMouseButton;


    glm::vec2 scroll{};
    glm::vec2 scroll_offset{};

    glm::dvec2 last_cursor{};
    glm::dvec2 cursor{};
    glm::dvec2 cursor_offset{};

    std::map<glfw::KeyCode, bool> pressed_keys;
    std::map<glfw::KeyCode, bool> up_keys;
    std::map<glfw::KeyCode, bool> down_keys;

    std::map<glfw::MouseButton, bool> pressed_mouse_buttons;
    std::map<glfw::MouseButton, bool> up_mouse_buttons;
    std::map<glfw::MouseButton, bool> down_mouse_buttons;

    std::queue<UnhandledKey> unhandled_keys;
    std::queue<UnhandledMouseButton> unhandled_mouse_buttons;

    glm::vec2 current_mouse{};
    glm::vec2 last_mouse{};
    glm::vec2 offset_mouse{};

    bool Input::GetKeyPressed(const glfw::KeyCode code) {
        return pressed_keys[code];
    }

    bool Input::GetKeyDown(const glfw::KeyCode code) {
        return down_keys[code];
    }

    bool Input::GetKeyUp(const glfw::KeyCode code) {
        return up_keys[code];
    }

    bool Input::GetMouseButtonPressed(const glfw::MouseButton button) {
        return pressed_mouse_buttons[button];
    }

    bool Input::GetMouseButtonDown(const glfw::MouseButton button) {
        return down_mouse_buttons[button];
    }

    bool Input::GetMouseButtonUp(const glfw::MouseButton button) {
        return up_mouse_buttons[button];
    }

    const glm::vec2& Input::GetCursorPosition() {
        return current_mouse;
    }

    const glm::vec2& Input::GetCursorOffset() {
        return offset_mouse;
    }

    const glm::vec2& Input::GetScrollOffset() {
        return scroll_offset;
    }

    void Input::Update() {
        down_keys.clear();
        up_keys.clear();
        down_mouse_buttons.clear();
        up_mouse_buttons.clear();

        while (!unhandled_keys.empty()) {
            auto [code, action] = unhandled_keys.front();
            unhandled_keys.pop();

            if (action == glfw::KeyAction::Press) {
                down_keys[code] = true;
                pressed_keys[code] = true;
            }
            else if (action == glfw::KeyAction::Release) {
                up_keys[code] = true;
                pressed_keys[code] = false;
            }
        }

        while (!unhandled_mouse_buttons.empty()) {
            auto [code, action] = unhandled_mouse_buttons.front();
            unhandled_mouse_buttons.pop();

            if (action == glfw::MouseButtonAction::Press) {
                down_mouse_buttons[code] = true;
                pressed_mouse_buttons[code] = true;
            }
            else if (action == glfw::MouseButtonAction::Release) {
                up_mouse_buttons[code] = true;
                pressed_mouse_buttons[code] = false;
            }
        }

        offset_mouse = current_mouse - last_mouse;
        last_mouse = current_mouse;

        scroll_offset = scroll;
        scroll = glm::zero<glm::vec2>();
    }

    void Input::Clear() {
        pressed_keys.clear();
        up_keys.clear();
        down_keys.clear();
    }

    void Input::InitCursor(const float x, const float y) {
        current_mouse = glm::vec2(x, y);
        last_mouse = glm::vec2(x, y);
    }

    void Input::HandleKey(const glfw::KeyCode code, const glfw::KeyAction action) {
        unhandled_keys.push({code, action});
    }

    void Input::HandleMouseButton(const glfw::MouseButton button, const glfw::MouseButtonAction action) {
        unhandled_mouse_buttons.push({button, action});
    }

    void Input::HandleCursorPosition(const float x, const float y) {
        current_mouse.x = x;
        current_mouse.y = y;
    }

    void Input::HandleScroll(const float x, const float y) {
        scroll = {x, y};
    }
}
