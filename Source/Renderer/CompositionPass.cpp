#include "CompositionPass.h"

#include "Gpu/GraphicsContext.h"

#include "Assets/TextAsset.h"

CompositionPass::CompositionPass() {
  TextAsset vsCode("Assets/Shaders/Composition/Composition.vs");
  TextAsset fsCode("Assets/Shaders/Composition/Composition.fs");
  shader = std::make_shared<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ColorPassResult", "u_LightPassResult");
  shader->Bind();
    shader->SetInt("u_ColorPassResult", 0);
    shader->SetInt("u_LightPassResult", 1);
  shader->Unbind();
  canvas = std::make_shared<Vao>(Primitives::Canvas::vertices, Vertex::GetLayout(), Primitives::Canvas::indices);
}

void CompositionPass::Execute(std::shared_ptr<ColorPass>& colorPass, std::shared_ptr<LightPass>& lightPass) {
	GraphicsContext::ClearColor(Color (227.0f, 251.0f, 255.0f, 255.0f) / 255.0f);
  GraphicsContext::Clear();
  shader->Bind();
    canvas->Bind();
      colorPass->GetFbo()->BindTexture(GL_TEXTURE0);
      lightPass->GetFbo()->BindTexture(GL_TEXTURE0 + 1);
        glDrawElements(GL_TRIANGLES, canvas->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
      lightPass->GetFbo()->UnbindTexture(GL_TEXTURE0 + 1);
      colorPass->GetFbo()->UnbindTexture(GL_TEXTURE0);
    canvas->Unbind();
  shader->Unbind();
}