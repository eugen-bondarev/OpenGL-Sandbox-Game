#include "GraphicsContext.h"

namespace Werwel {
namespace GraphicsContext {

void DrawIndexed(int amountOfIndices) {
  glDrawElements(GL_TRIANGLES, amountOfIndices, GL_UNSIGNED_INT, nullptr);
}

}
}