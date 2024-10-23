#pragma once

#include "gl_common.hpp"

namespace glfwxx {
    enum class BufferUsage {
#ifdef GL_STREAM_DRAW
        StreamDraw = GL_STREAM_DRAW,
#endif
#ifdef GL_STREAM_READ
        StreamRead = GL_STREAM_READ,
#endif
#ifdef GL_STREAM_COPY
        StreamCopy = GL_STREAM_COPY,
#endif
#ifdef GL_STATIC_DRAW
        StaticDraw = GL_STATIC_DRAW,
#endif
#ifdef GL_STATIC_READ
        StaticRead = GL_STATIC_READ,
#endif
#ifdef GL_STATIC_COPY
        StaticCopy = GL_STATIC_COPY,
#endif
#ifdef GL_DYNAMIC_DRAW
        DynamicDraw = GL_DYNAMIC_DRAW,
#endif
#ifdef GL_DYNAMIC_READ
        DynamicRead = GL_DYNAMIC_READ,
#endif
#ifdef GL_DYNAMIC_COPY
        DynamicCopy = GL_DYNAMIC_COPY,
#endif
    };
}