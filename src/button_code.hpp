#pragma once

#include "gl_common.hpp"

namespace glfwxx {
    enum class ButtonCode : GLenum {
#ifdef GLFW_MOUSE_BUTTON_LEFT
        Left = GLFW_MOUSE_BUTTON_LEFT,
#endif
#ifdef GLFW_MOUSE_BUTTON_RIGHT
        Right = GLFW_MOUSE_BUTTON_RIGHT,
#endif
#ifdef GLFW_MOUSE_BUTTON_MIDDLE
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
#endif
#ifdef GLFW_MOUSE_BUTTON_1
        Button1 = GLFW_MOUSE_BUTTON_1,
#endif
#ifdef GLFW_MOUSE_BUTTON_2
        Button2 = GLFW_MOUSE_BUTTON_2,
#endif
#ifdef GLFW_MOUSE_BUTTON_3
        Button3 = GLFW_MOUSE_BUTTON_3,
#endif
#ifdef GLFW_MOUSE_BUTTON_4
        Button4 = GLFW_MOUSE_BUTTON_4,
#endif
#ifdef GLFW_MOUSE_BUTTON_5
        Button5 = GLFW_MOUSE_BUTTON_5,
#endif
#ifdef GLFW_MOUSE_BUTTON_6
        Button6 = GLFW_MOUSE_BUTTON_6,
#endif
#ifdef GLFW_MOUSE_BUTTON_7
        Button7 = GLFW_MOUSE_BUTTON_7,
#endif
#ifdef GLFW_MOUSE_BUTTON_8
        Button8 = GLFW_MOUSE_BUTTON_8,
#endif
    };
}