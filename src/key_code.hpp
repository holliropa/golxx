#pragma once

#include "gl_common.hpp"

namespace glfwxx {
    enum class KeyCode : GLenum {
#ifdef GLFW_KEY_SPACE
        Space = GLFW_KEY_SPACE,
#endif
#ifdef GLFW_KEY_APOSTROPHE
        Apostrophe = GLFW_KEY_APOSTROPHE,
#endif
#ifdef GLFW_KEY_COMMA
        Comma = GLFW_KEY_COMMA,
#endif
#ifdef GLFW_KEY_MINUS
        Minus = GLFW_KEY_MINUS,
#endif
#ifdef GLFW_KEY_PERIOD
        Period = GLFW_KEY_PERIOD,
#endif
#ifdef GLFW_KEY_SLASH
        Slash = GLFW_KEY_SLASH,
#endif
#ifdef GLFW_KEY_0
        Zero = GLFW_KEY_0,
#endif
#ifdef GLFW_KEY_1
        One = GLFW_KEY_1,
#endif
#ifdef GLFW_KEY_2
        Two = GLFW_KEY_2,
#endif
#ifdef GLFW_KEY_3
        Three = GLFW_KEY_3,
#endif
#ifdef GLFW_KEY_4
        Four = GLFW_KEY_4,
#endif
#ifdef GLFW_KEY_5
        Five = GLFW_KEY_5,
#endif
#ifdef GLFW_KEY_6
        Six = GLFW_KEY_6,
#endif
#ifdef GLFW_KEY_7
        Seven = GLFW_KEY_7,
#endif
#ifdef GLFW_KEY_8
        Eight = GLFW_KEY_8,
#endif
#ifdef GLFW_KEY_9
        Nine = GLFW_KEY_9,
#endif
#ifdef GLFW_KEY_SEMICOLON
        Semicolon = GLFW_KEY_SEMICOLON,
#endif
#ifdef GLFW_KEY_EQUAL
        Equal = GLFW_KEY_EQUAL,
#endif
#ifdef GLFW_KEY_A
        A = GLFW_KEY_A,
#endif
#ifdef GLFW_KEY_B
        B = GLFW_KEY_B,
#endif
#ifdef GLFW_KEY_C
        C = GLFW_KEY_C,
#endif
#ifdef GLFW_KEY_D
        D = GLFW_KEY_D,
#endif
#ifdef GLFW_KEY_E
        E = GLFW_KEY_E,
#endif
#ifdef GLFW_KEY_F
        F = GLFW_KEY_F,
#endif
#ifdef GLFW_KEY_G
        G = GLFW_KEY_G,
#endif
#ifdef GLFW_KEY_H
        H = GLFW_KEY_H,
#endif
#ifdef GLFW_KEY_I
        I = GLFW_KEY_I,
#endif
#ifdef GLFW_KEY_J
        J = GLFW_KEY_J,
#endif
#ifdef GLFW_KEY_K
        K = GLFW_KEY_K,
#endif
#ifdef GLFW_KEY_L
        L = GLFW_KEY_L,
#endif
#ifdef GLFW_KEY_M
        M = GLFW_KEY_M,
#endif
#ifdef GLFW_KEY_N
        N = GLFW_KEY_N,
#endif
#ifdef GLFW_KEY_O
        O = GLFW_KEY_O,
#endif
#ifdef GLFW_KEY_P
        P = GLFW_KEY_P,
#endif
#ifdef GLFW_KEY_Q
        Q = GLFW_KEY_Q,
#endif
#ifdef GLFW_KEY_R
        R = GLFW_KEY_R,
#endif
#ifdef GLFW_KEY_S
        S = GLFW_KEY_S,
#endif
#ifdef GLFW_KEY_T
        T = GLFW_KEY_T,
#endif
#ifdef GLFW_KEY_U
        U = GLFW_KEY_U,
#endif
#ifdef GLFW_KEY_V
        V = GLFW_KEY_V,
#endif
#ifdef GLFW_KEY_W
        W = GLFW_KEY_W,
#endif
#ifdef GLFW_KEY_X
        X = GLFW_KEY_X,
#endif
#ifdef GLFW_KEY_Y
        Y = GLFW_KEY_Y,
#endif
#ifdef GLFW_KEY_Z
        Z = GLFW_KEY_Z,
#endif
#ifdef GLFW_KEY_LEFT_BRACKET
        LeftBracket = GLFW_KEY_LEFT_BRACKET,
#endif
#ifdef GLFW_KEY_BACKSLASH
        Backslash = GLFW_KEY_BACKSLASH,
#endif
#ifdef GLFW_KEY_RIGHT_BRACKET
        RightBracket = GLFW_KEY_RIGHT_BRACKET,
#endif
#ifdef GLFW_KEY_GRAVE_ACCENT
        GraveAccent = GLFW_KEY_GRAVE_ACCENT,
#endif
#ifdef GLFW_KEY_ESCAPE
        Escape = GLFW_KEY_ESCAPE,
#endif
#ifdef GLFW_KEY_ENTER
        Enter = GLFW_KEY_ENTER,
#endif
#ifdef GLFW_KEY_TAB
        Tab = GLFW_KEY_TAB,
#endif
#ifdef GLFW_KEY_BACKSPACE
        Backspace = GLFW_KEY_BACKSPACE,
#endif
#ifdef GLFW_KEY_INSERT
        Insert = GLFW_KEY_INSERT,
#endif
#ifdef GLFW_KEY_DELETE
        Delete = GLFW_KEY_DELETE,
#endif
#ifdef GLFW_KEY_RIGHT
        Right = GLFW_KEY_RIGHT,
#endif
#ifdef GLFW_KEY_LEFT
        Left = GLFW_KEY_LEFT,
#endif
#ifdef GLFW_KEY_DOWN
        Down = GLFW_KEY_DOWN,
#endif
#ifdef GLFW_KEY_UP
        Up = GLFW_KEY_UP,
#endif
#ifdef GLFW_KEY_PAGE_UP
        PageUp = GLFW_KEY_PAGE_UP,
#endif
#ifdef GLFW_KEY_PAGE_DOWN
        PageDown = GLFW_KEY_PAGE_DOWN,
#endif
#ifdef GLFW_KEY_HOME
        Home = GLFW_KEY_HOME,
#endif
#ifdef GLFW_KEY_END
        End = GLFW_KEY_END,
#endif
#ifdef GLFW_KEY_CAPS_LOCK
        CapsLock = GLFW_KEY_CAPS_LOCK,
#endif
#ifdef GLFW_KEY_SCROLL_LOCK
        ScrollLock = GLFW_KEY_SCROLL_LOCK,
#endif
#ifdef GLFW_KEY_NUM_LOCK
        NumLock = GLFW_KEY_NUM_LOCK,
#endif
#ifdef GLFW_KEY_PRINT_SCREEN
        PrintScreen = GLFW_KEY_PRINT_SCREEN,
#endif
#ifdef GLFW_KEY_PAUSE
        Pause = GLFW_KEY_PAUSE,
#endif
#ifdef GLFW_KEY_F1
        F1 = GLFW_KEY_F1,
#endif
#ifdef GLFW_KEY_F2
        F2 = GLFW_KEY_F2,
#endif
#ifdef GLFW_KEY_F3
        F3 = GLFW_KEY_F3,
#endif
#ifdef GLFW_KEY_F4
        F4 = GLFW_KEY_F4,
#endif
#ifdef GLFW_KEY_F5
        F5 = GLFW_KEY_F5,
#endif
#ifdef GLFW_KEY_F6
        F6 = GLFW_KEY_F6,
#endif
#ifdef GLFW_KEY_F7
        F7 = GLFW_KEY_F7,
#endif
#ifdef GLFW_KEY_F8
        F8 = GLFW_KEY_F8,
#endif
#ifdef GLFW_KEY_F9
        F9 = GLFW_KEY_F9,
#endif
#ifdef GLFW_KEY_F10
        F10 = GLFW_KEY_F10,
#endif
#ifdef GLFW_KEY_F11
        F11 = GLFW_KEY_F11,
#endif
#ifdef GLFW_KEY_F12
        F12 = GLFW_KEY_F12,
#endif
#ifdef GLFW_KEY_KP_0
        KeyPad0 = GLFW_KEY_KP_0,
#endif
#ifdef GLFW_KEY_KP_1
        KeyPad1 = GLFW_KEY_KP_1,
#endif
#ifdef GLFW_KEY_KP_2
        KeyPad2 = GLFW_KEY_KP_2,
#endif
#ifdef GLFW_KEY_KP_3
        KeyPad3 = GLFW_KEY_KP_3,
#endif
#ifdef GLFW_KEY_KP_4
        KeyPad4 = GLFW_KEY_KP_4,
#endif
#ifdef GLFW_KEY_KP_5
        KeyPad5 = GLFW_KEY_KP_5,
#endif
#ifdef GLFW_KEY_KP_6
        KeyPad6 = GLFW_KEY_KP_6,
#endif
#ifdef GLFW_KEY_KP_7
        KeyPad7 = GLFW_KEY_KP_7,
#endif
#ifdef GLFW_KEY_KP_8
        KeyPad8 = GLFW_KEY_KP_8,
#endif
#ifdef GLFW_KEY_KP_9
        KeyPad9 = GLFW_KEY_KP_9,
#endif
#ifdef GLFW_KEY_KP_DECIMAL
        KeyPadDecimal = GLFW_KEY_KP_DECIMAL,
#endif
#ifdef GLFW_KEY_KP_DIVIDE
        KeyPadDivide = GLFW_KEY_KP_DIVIDE,
#endif
#ifdef GLFW_KEY_KP_MULTIPLY
        KeyPadMultiply = GLFW_KEY_KP_MULTIPLY,
#endif
#ifdef GLFW_KEY_KP_SUBTRACT
        KeyPadSubtract = GLFW_KEY_KP_SUBTRACT,
#endif
#ifdef GLFW_KEY_KP_ADD
        KeyPadAdd = GLFW_KEY_KP_ADD,
#endif
#ifdef GLFW_KEY_KP_ENTER
        KeyPadEnter = GLFW_KEY_KP_ENTER,
#endif
#ifdef GLFW_KEY_LEFT_SHIFT
        LeftShift = GLFW_KEY_LEFT_SHIFT,
#endif
#ifdef GLFW_KEY_LEFT_CONTROL
        LeftControl = GLFW_KEY_LEFT_CONTROL,
#endif
#ifdef GLFW_KEY_LEFT_ALT
        LeftAlt = GLFW_KEY_LEFT_ALT,
#endif
#ifdef GLFW_KEY_RIGHT_SHIFT
        RightShift = GLFW_KEY_RIGHT_SHIFT,
#endif
#ifdef GLFW_KEY_RIGHT_CONTROL
        RightControl = GLFW_KEY_RIGHT_CONTROL,
#endif
#ifdef GLFW_KEY_RIGHT_ALT
        RightAlt = GLFW_KEY_RIGHT_ALT,
#endif
#ifdef GLFW_KEY_MENU
        Menu = GLFW_KEY_MENU
#endif
    };

} // namespace glfwxx
