#pragma once

#include "Math/Math.h"

#include <GL/glew.h>

namespace GraphicsContext {

inline static void ClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a = 1.0f) {
	glClearColor(r, g, b, a);
}

inline static void Clear(GLbitfield mask = GL_COLOR_BUFFER_BIT) {
	glClear(mask);
}

inline static void Viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	glViewport(x, y, width, height);
}

}