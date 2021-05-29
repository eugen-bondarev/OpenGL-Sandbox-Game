#include "CompositionPass.h"

#include "Gpu/GraphicsContext.h"

#include "Assets/TextAsset.h"

CompositionPass::CompositionPass() {
  TextAsset vsCode("Assets/Shaders/Composition/Composition.vs");
  TextAsset fsCode("Assets/Shaders/Composition/Composition.fs");
  shader = std::make_shared<Shader>(vsCode.GetContent(), fsCode.GetContent());
  canvas = std::make_shared<Vao>(Primitives::Canvas::vertices, Vertex::GetLayout(), Primitives::Canvas::indices);
}

void CompositionPass::Execute(std::shared_ptr<ColorPass>& colorPass) {
  GraphicsContext::Clear();
  shader->Bind();
    canvas->Bind();
      colorPass->GetFbo()->BindTexture();
        glDrawElements(GL_TRIANGLES, canvas->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
      colorPass->GetFbo()->UnbindTexture();
    canvas->Unbind();
  shader->Unbind();
}