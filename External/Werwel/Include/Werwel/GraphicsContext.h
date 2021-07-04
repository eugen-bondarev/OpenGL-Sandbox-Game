#pragma once

#include <GL/glew.h>

namespace Werwel {

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

inline static void EnableTransparency() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DrawIndexed(int amountOfIndices);

}

}