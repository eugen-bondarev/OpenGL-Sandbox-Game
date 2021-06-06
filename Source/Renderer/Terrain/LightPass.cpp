#include "LightPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "GPU/GraphicsContext.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

LightPass::LightPass(Ref<MapRenderer>& mapRenderer) {  
	this->mapRenderer = mapRenderer;

	const TextAsset chunkShaderVsCode(
		"Assets/Shaders/Terrain/Light"
		#ifdef FORGIO_SUPPORT_DYNAMIC_BUFFER
			".dyn.vs"
		#else
			".vs"
		#endif
	);

	const TextAsset chunkShaderFsCode("Assets/Shaders/Terrain/Light.fs");
	
	shader = CreateRef<Shader>(
		chunkShaderVsCode.GetContent(), 
		chunkShaderFsCode.GetContent(), 
		"u_Proj", 
		"u_View"
	#ifndef FORGIO_SUPPORT_DYNAMIC_BUFFER
		, "u_Positions"
	#endif
	);

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

		#ifdef FORGIO_SUPPORT_DYNAMIC_BUFFER
			transformationVBO = lightVao->AddVBO(
				VBO::Type::Array, 
				VBO::Usage::Stream, 
				maxAmountOfLights, sizeof(Vec2), nullptr, 
				// 0, sizeof(Vec2), nullptr, 
				std::vector<VertexBufferLayout> { { 2, sizeof(Vec2), 0, 1 } }
			);
		#endif
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
			#ifdef FORGIO_SUPPORT_DYNAMIC_BUFFER
				transformationVBO->Update(lightData, std::min<int>(lightData.size(), maxAmountOfLights));
				// transformationVBO->Store(lightData);
			#else
				shader->SetListVec2("u_Positions", lightData);
			#endif
      lightVao->Bind();			
			lightVao->GetIndexBuffer()->Bind();
				lightTexture->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, lightVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr, lightData.size());
				lightTexture->Unbind();
      lightVao->Unbind();
    shader->Unbind();
  fbo->Unbind();
}