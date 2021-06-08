#include "CompositionPass.h"

#include "GPU/GraphicsContext.h"

#include "Assets/TextAsset.h"

CompositionPass::CompositionPass() {
  TextAsset vsCode("Assets/Shaders/Composition/Composition.vs");
  TextAsset fsCode("Assets/Shaders/Composition/Composition.fs");
  shader = CreateRef<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ColorPassResult", "u_LightPassResult");
  shader->Bind();
    shader->SetInt("u_ColorPassResult", 0);
    shader->SetInt("u_LightPassResult", 1);
  shader->Unbind();

	const auto& vertices = Primitives::Canvas::vertices;
	const auto& indices = Primitives::Canvas::indices;

  // canvas = CreateRef<VAO>(Primitives::Canvas::vertices, Vertex::GetLayout(), Primitives::Canvas::indices);
  canvas = CreateRef<VAO>();
  canvas->Bind();
		canvas->AddVBO(VBO::Type::Array, VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		canvas->AddVBO(VBO::Type::Indices, VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
  canvas->Unbind();
}

void CompositionPass::Execute(Ref<ColorPass>& colorPass, Ref<LightPass>& lightPass) {
  FORGIO_PROFILER_SCOPE();

  Color sky = Color(227.0f, 251.0f, 255.0f, 255.0f) / 255.0f;

	GraphicsContext::ClearColor(sky.r, sky.g, sky.b, sky.a);
  GraphicsContext::Clear();
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