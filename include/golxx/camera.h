#pragma once

#include "glm_common.h"

namespace golxx {
    class Camera {
    public:
        glm::vec3 position{};

        Camera(const float zoom_level, const glm::vec2 size)
            : zoom_level_(zoom_level),
              size_(size) {
            update_projection();
        }

        [[nodiscard]] const glm::mat4& get_projection() const {
            return projection_;
        }

        [[nodiscard]] float get_zoom_level() const {
            return zoom_level_;
        }

        [[nodiscard]] const glm::vec2& get_size() const {
            return size_;
        }

        void set_zoom_level(const float zoom_level) {
            zoom_level_ = zoom_level;
            update_projection();
        }

        void set_size(const glm::vec2 size) {
            size_ = size;
            update_projection();
        }

        [[nodiscard]] glm::vec2 cursor_to_world(const glm::vec2 cursor) const {
            const glm::vec4 ndcPos(
                2.0f * cursor.x / static_cast<float>(size_.x) - 1.0f,
                1.0f - 2.0f * cursor.y / static_cast<float>(size_.y),
                0.0f,
                1.0f);

            const glm::mat4 inverseProjView = glm::inverse(
                projection_ * glm::translate(glm::identity<glm::mat4>(), -position));
            glm::vec4 worldPos = inverseProjView * ndcPos;

            worldPos /= worldPos.w;

            return {worldPos.x, worldPos.y};
        }

    private:
        void update_projection() {
            const auto aspect_ratio = size_.x / size_.y;

            projection_ = glm::ortho(
                -1.0f * aspect_ratio * zoom_level_,
                1.0f * aspect_ratio * zoom_level_,
                -zoom_level_,
                zoom_level_
            );
        }

    private:
        float zoom_level_;
        glm::mat4 projection_;
        glm::vec2 size_;
    };
}
