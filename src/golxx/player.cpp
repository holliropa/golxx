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

    bool get_cell_state(const NodePtr& node, const int x, const int y) {
        if (!node) return false;

        if (node->level == 0) {
            return node->alive;
        }

        const int half_size = 1 << (node->level - 1);

        if (x >= 0 && y >= 0) {
            return get_cell_state(node->ne, x - half_size, y - half_size);
        }

        if (x >= 0) {
            return get_cell_state(node->se, x - half_size, y + half_size);
        }

        if (y >= 0) {
            return get_cell_state(node->nw, x + half_size, y - half_size);
        }

        return get_cell_state(node->sw, x + half_size, y + half_size);
    }

    NodePtr ensure_tree_size(NodePtr root, int x, int y) {
        if (!root) {
            const auto dead = make_leaf(false);
            root = make_node(dead, dead, dead, dead);
        }

        while (root) {
            const int half_size = 1 << (root->level - 1);
            if (x >= -half_size && x < half_size && y >= -half_size && y < half_size) {
                break;
            }

            auto dead = make_leaf(false);
            auto empty_quadrant = root;

            for (int i = 0; i < root->level - 1; ++i) {
                empty_quadrant = make_node(dead, dead, dead, dead);
                dead = empty_quadrant;
            }

            // Determine which quadrant the current root should be in the expanded tree
            if (x >= 0 && y >= 0) {
                // Current root becomes SW, expand to NE
                root = make_node(empty_quadrant, empty_quadrant, root, empty_quadrant);
            }
            else if (x >= 0) {
                // Current root becomes NW, expand to SE
                root = make_node(root, empty_quadrant, empty_quadrant, empty_quadrant);
            }
            else if (y >= 0) {
                // Current root becomes SE, expand to NW
                root = make_node(empty_quadrant, empty_quadrant, empty_quadrant, root);
            }
            else {
                // Current root becomes NE, expand to SW
                root = make_node(empty_quadrant, root, empty_quadrant, empty_quadrant);
            }
        }

        return root;
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

            bool current_state = false;
            if (simulator_->root) {
                current_state = get_cell_state(simulator_->root, current_cell.x, current_cell.y);
            }

            drawing_state_ = !current_state;

            simulator_->root = ensure_tree_size(simulator_->root, current_cell.x, current_cell.y);
            simulator_->root = set(simulator_->root, current_cell.x, current_cell.y, drawing_state_);
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
            simulator_->root = ensure_tree_size(simulator_->root, current.x, current.y);
            simulator_->root = set(simulator_->root, current.x, current.y, toggle);

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
