#include "graphics_context.h"

namespace mw {
namespace GraphicsContext {

void ClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
	glClearColor(r, g, b, a);
}

void Clear(GLbitfield mask)
{
	glClear(mask);
}

void Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glViewport(x, y, width, height);
}

void EnableTransparency()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DrawIndexed(int amountOfIndices) 
{
  glDrawElements(GL_TRIANGLES, amountOfIndices, GL_UNSIGNED_INT, nullptr);
}

}
}