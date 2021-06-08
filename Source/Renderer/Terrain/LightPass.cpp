#include "LightPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "GPU/GraphicsContext.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

LightPass::LightPass(Ref<MapRenderer>& mapRenderer) {  
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
			vertices.size(), sizeof(Vertex2D), &vertices[0], 
			Vertex2D::GetLayout()
		);

		lightVao->AddVBO(
			VBO::Type::Indices, 
			VBO::Usage::Static, 
			indices.size(), sizeof(int), &indices[0]
		);

		transformationVBO = lightVao->AddVBO(
			VBO::Type::Array, 
			VBO::Usage::Stream, 
			maxAmountOfLights, sizeof(Vec2), nullptr,
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
		Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);
}

void LightPass::Execute(const Mat4& viewMatrix, const Vec2& viewPos, const light_data_t& lightData) {
  FORGIO_PROFILER_SCOPE();

	GraphicsContext::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (!lightData.size()) return;

  fbo->Bind();
  fbo->Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
			transformationVBO->Update(lightData, std::min<int>(lightData.size(), maxAmountOfLights));
      lightVao->Bind();			
			lightVao->GetIndexBuffer()->Bind();
				lightTexture->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, lightVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr, lightData.size());
				lightTexture->Unbind();
      lightVao->Unbind();
    shader->Unbind();
  fbo->Unbind();
}