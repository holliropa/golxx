#pragma once

#include <iostream>

#include "gl_common.h"

class FPSCounter {
private:
    unsigned frameCount_ = 0;
    double lastUpdateTime_ = 0.0;
    double lastFPS_ = 0.0;

public:
    void update() {
        frameCount_++;
        const auto currentTime = glfw::getTime();
        const auto timeSinceUpdate = currentTime - lastUpdateTime_;

        if (timeSinceUpdate > 1.0) {
            const auto fps = static_cast<double>(frameCount_) / timeSinceUpdate;
            // std::cout << "FPS: " << fps << '\n';
            lastUpdateTime_ = currentTime;
            frameCount_ = 0;
            lastFPS_ = fps;
        }
    }

    unsigned getFrameCount() const { return frameCount_; }

    double getFPS() const { return lastFPS_; }
};
