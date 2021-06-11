#include "CompositionPass.h"

#include "Werwel/GraphicsContext.h"

#include "Assets/TextAsset.h"

CompositionPass::CompositionPass() {
  TextAsset vsCode("Assets/Shaders/Composition/Composition.vs");
  TextAsset fsCode("Assets/Shaders/Composition/Composition.fs");
  shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ColorPassResult", "u_LightPassResult");
  shader->Bind();
    shader->SetInt("u_ColorPassResult", 0);
    shader->SetInt("u_LightPassResult", 1);
  shader->Unbind();

	const auto& vertices = Primitives::Canvas::vertices;
	const auto& indices = Primitives::Canvas::indices;

  canvas = CreateRef<Werwel::VAO>();
  canvas->Bind();
		canvas->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		canvas->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
  canvas->Unbind();
}

void CompositionPass::Execute(Ref<ColorPass>& colorPass, Ref<LightPass>& lightPass) {
  FORGIO_PROFILER_SCOPE();

  Color sky = Color(209, 247, 255, 255.0f) / 255.0f;
  Color night = Color(1, 5, 10, 255.0f) / 255.0f;

  Color current = sky;

	Werwel::GraphicsContext::ClearColor(current.r, current.g, current.b, sky.a);
  Werwel::GraphicsContext::Clear();
  shader->Bind();
    canvas->Bind();
    canvas->GetIndexBuffer()->Bind();
      colorPass->GetFbo()->BindTexture(GL_TEXTURE0);
      lightPass->GetFbo()->BindTexture(GL_TEXTURE0 + 1);
        glDrawElements(GL_TRIANGLES, canvas->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
      lightPass->GetFbo()->UnbindTexture(GL_TEXTURE0 + 1);
      colorPass->GetFbo()->UnbindTexture(GL_TEXTURE0);
    canvas->Unbind();
  shader->Unbind();
}