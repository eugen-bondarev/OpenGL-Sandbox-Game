#include "CompositionPass.h"

CompositionPass::CompositionPass() {
  TextAsset vsCode("Assets/Shaders/Terrain/CompositionShader.vs");
  TextAsset fsCode("Assets/Shaders/Terrain/CompositionShader.fs");
  shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ColorPassResult", "u_LightPassResult");
  shader->Bind();
    shader->SetInt("u_ColorPassResult", 0);
    shader->SetInt("u_LightPassResult", 1);
  shader->Unbind();
  
  const auto& vers = Primitives::Block::Vertices(2, -2);
  const auto& inds = Primitives::Block::indices;

  canvas = CreateRef<Werwel::VAO>();
  canvas->BindSafely();
  canvas->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
  canvas->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);
}

void CompositionPass::Perform(const Ref<ColorPass>& colorPass, const Ref<LightPass>& lightPass) {   
  FORGIO_PROFILER_SCOPE();

  static Color sky = Color(209, 247, 255, 255.0f) / 255.0f;
  Werwel::GraphicsContext::ClearColor(sky.r, sky.g, sky.b, sky.a);
  Werwel::GraphicsContext::Clear();

  shader->Bind();
    canvas->Bind();
    canvas->GetIndexBuffer()->Bind();
      colorPass->fbo->GetTexture()->Bind(GL_TEXTURE0);
      lightPass->fbo->GetTexture()->Bind(GL_TEXTURE0 + 1);
        glDrawElements(GL_TRIANGLES, canvas->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
      glActiveTexture(GL_TEXTURE0);

  FORGIO_SYNC_GPU();
}