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
