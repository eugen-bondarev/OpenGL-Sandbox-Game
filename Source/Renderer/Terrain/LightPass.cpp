#include "LightPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "GPU/GraphicsContext.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

LightPass::LightPass(std::shared_ptr<MapRenderer>& mapRenderer) {  
	this->mapRenderer = mapRenderer;

	const TextAsset chunkShaderVsCode("Assets/Shaders/Terrain/Light.vs");
	const TextAsset chunkShaderFsCode("Assets/Shaders/Terrain/Light.fs");
	shader = CreateRef<Shader>(chunkShaderVsCode.GetContent(), chunkShaderFsCode.GetContent(), "u_Proj", "u_View");

	const auto& vertices = Primitives::Block::Vertices(256, 256);
	const auto& indices = Primitives::Block::indices;

	lightVao = CreateRef<VAO>();

	lightVao->Bind();
		lightVao->AddVBO(
			VBO::Type::Array, 
			VBO::Usage::Static, 
			vertices.size(), sizeof(Vertex), &vertices[0], 
			Vertex::GetLayout()
		);

		lightVao->AddVBO(
			VBO::Type::Indices, 
			VBO::Usage::Static, 
			indices.size(), sizeof(int), &indices[0]
		);

		transformationVBO = lightVao->AddVBO(
			VBO::Type::Array, 
			VBO::Usage::Stream, 
			0, sizeof(Vec2), nullptr, 
			std::vector<VertexBufferLayout> { { 2, sizeof(Vec2), 0, 1 } }
		);
	lightVao->Unbind();

	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	shader->Unbind();

  fbo = CreateRef<LightFBO>(Window::GetSize());

	const ImageAsset lightTextureAsset("Assets/Images/LightMask5.png");
	lightTexture = CreateRef<Texture>(
		lightTextureAsset.GetSize(),
		lightTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		std::vector<Texture::param_t> {
			{ ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
			{ ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
		}
	);
}

void LightPass::Execute(Ref<ColorPass>& colorPass, Ref<Map>& map, const Mat4& viewMatrix, const Vec2& viewPos, bool chunksChanged) {
	GraphicsContext::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (!colorPass->light.size()) return;

  fbo->Bind();
  fbo->Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));

			transformationVBO->Store(colorPass->light);

      lightVao->Bind();			
			lightVao->GetIndexBuffer()->Bind();

				lightTexture->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, lightVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr, colorPass->light.size());
				lightTexture->Unbind();

      lightVao->Unbind();
    shader->Unbind();
  fbo->Unbind();
}