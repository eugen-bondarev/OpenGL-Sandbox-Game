#pragma once

#include "Math/Math.h"

#include <GL/glew.h>

namespace GraphicsContext {

inline static void ClearColor(Color color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

inline static void Clear(GLbitfield mask = GL_COLOR_BUFFER_BIT) {
    glClear(mask);
}

}