#pragma once

#include <GL/glew.h>

namespace ww {
namespace GraphicsContext {

void ClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a = 1.0f);
void Clear(GLbitfield mask = GL_COLOR_BUFFER_BIT);

void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

void EnableTransparency();

void DrawIndexed(int amountOfIndices);

}
}