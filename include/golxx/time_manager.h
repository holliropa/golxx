#pragma once

#include "gl_common.h"

class TimeManager {
private:
    double lastTime_ = glfw::getTime();
    
public:
    float getDeltaTime() {
        const auto currentTime = glfw::getTime();
        const auto delta = static_cast<float>(currentTime - lastTime_);
        lastTime_ = currentTime;
        return delta;
    }
};