#include "WoodsRenderer.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Maths/Primitive.h"

WoodsRenderer::WoodsRenderer(const Ref<Woods>& woods, const Ref<Camera>& camera) : woods { woods }, camera { camera } {
  TextAsset vsCode("Assets/Shaders/Woods/VS_Woods.glsl");
  TextAsset fsCode("Assets/Shaders/Woods/FS_Woods.glsl");

  pipeline.shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ProjectionView");

	const auto& vers = Primitives::Block::Vertices(16, 204);
	const auto& inds = Primitives::Block::indices;

	pipeline.vao = CreateRef<Werwel::VAO>();
	pipeline.vao->BindSafely();
		pipeline.vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
		pipeline.vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);		
		pipeline.vbo = pipeline.vao->AddVBO(
			Werwel::VBO::Type::Array, 
			Werwel::VBO::Usage::Stream, 
			0, 
			sizeof(Vec2), 
			nullptr, 
			std::vector<Werwel::VertexBufferLayout> { 
				{ 2, sizeof(Vec2), 0, 1 }
			}
		);

  std::vector<Vec2> positions;

  for (int i = 0; i < woods->GetTrees().size(); i++) {
    positions.push_back(woods->GetTrees()[i].GetPosition());
  }

  pipeline.vbo->Bind();
    pipeline.vbo->Store(positions);

	const ImageAsset tileMapTexture("Assets/Images/Bark.png");
	pipeline.texture = CreateRef<Werwel::Texture>(
		tileMapTexture.GetSize(),
		tileMapTexture.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT }
	);
}

void WoodsRenderer::Render() {
	Mat4 projView = Window::GetSpace() * camera->GetTransform();

  visibleTrees.clear();
  woods->GetVisibleTrees(visibleTrees, camera);
  // LOG_OUT(visibleTrees.size());
  pipeline.vbo->Bind();
    pipeline.vbo->Store(visibleTrees);

  pipeline.shader->Bind();
  pipeline.shader->SetMat4x4("u_ProjectionView", Math::ToPtr(projView));
    pipeline.texture->Bind();
    pipeline.vao->Bind();
    pipeline.vao->GetIndexBuffer()->Bind();
      // pipeline.vbo->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, woods->GetTrees().size());
}