#include "golxx/player.h"
#include "golxx/input.h"

namespace golxx {
    glm::ivec2 get_movement_offset() {
        glm::ivec2 offset{};

        if (Input::GetKeyPressed(glfw::KeyCode::W)) {
            offset.y++;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::S)) {
            offset.y--;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::A)) {
            offset.x--;
        }
        if (Input::GetKeyPressed(glfw::KeyCode::D)) {
            offset.x++;
        }

        return offset;
    }

    Player::Player(const std::shared_ptr<Camera>& camera,
                   const std::shared_ptr<Simulator>& simulator,
                   const float speed)
        : camera_(camera),
          simulator_(simulator),
          speed_(speed) {}

    void Player::update(const float deltaTime) {
        if (const auto& movement = get_movement_offset(); movement != glm::zero<glm::ivec2>()) {
            camera_->position += glm::vec3(movement.x, movement.y, 0.0f) * speed_ * deltaTime;
        }

        const auto cursorWorldPos = camera_->cursor_to_world(Input::GetCursorPosition());

        if (const auto& scroll = Input::GetScrollOffset(); scroll.y != 0.0f) {
            const auto zoomLevel = camera_->get_zoom_level();
            const auto scrollOffset = scroll.y * std::max(0.1f * zoomLevel, 1.0f);
            const auto newZoomLevel = std::max(zoomLevel - scrollOffset, 1.0f);

            camera_->set_zoom_level(newZoomLevel);

            // Calculate world position after zoom change
            const auto worldPosAfter = camera_->cursor_to_world(Input::GetCursorPosition());

            // Adjust camera position to keep cursor at same world position
            const auto offset = cursorWorldPos - worldPosAfter;
            camera_->position += glm::vec3(offset.x, offset.y, 0.0f);
        }

        const auto current_cell = glm::ivec2(
            static_cast<int>(std::floor(cursorWorldPos.x)),
            static_cast<int>(std::floor(cursorWorldPos.y)));

        if (Input::GetMouseButtonDown(glfw::MouseButton::Left)) {
            is_drawing_line_ = true;
            last_cell_ = current_cell;
            drawing_state_ = simulator_->getCells().find(current_cell) == simulator_->getCells().end();
            simulator_->set_state(current_cell, drawing_state_);
        }
        else if (Input::GetMouseButtonUp(glfw::MouseButton::Left)) {
            is_drawing_line_ = false;
        }
        else if (is_drawing_line_ && current_cell != last_cell_) {
            toggle_line_cells(last_cell_, current_cell, drawing_state_);
            last_cell_ = current_cell;
        }
    }

    void Player::toggle_line_cells(const glm::ivec2 from, const glm::ivec2 to, const bool toggle) {
        const int dx = abs(to.x - from.x);
        const int dy = -abs(to.y - from.y);
        const int sx = from.x < to.x ? 1 : -1;
        const int sy = from.y < to.y ? 1 : -1;
        int err = dx + dy;

        glm::ivec2 current = from;
        while (true) {
            simulator_->set_state(current, toggle);

            if (current == to) break;

            const int e2 = 2 * err;
            if (e2 >= dy) {
                err += dy;
                current.x += sx;
            }
            if (e2 <= dx) {
                err += dx;
                current.y += sy;
            }
        }
    }
}
