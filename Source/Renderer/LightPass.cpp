#include "LightPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Gpu/GraphicsContext.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

LightPass::LightPass(std::shared_ptr<MapRenderer>& mapRenderer) {  
	this->mapRenderer = mapRenderer;

	const TextAsset chunkShaderVsCode("Assets/Shaders/Terrain/Light.vs");
	const TextAsset chunkShaderFsCode("Assets/Shaders/Terrain/Light.fs");
	shader = std::make_shared<Shader>(chunkShaderVsCode.GetContent(), chunkShaderFsCode.GetContent(), "u_Proj", "u_View", "u_Pos");

	lightVao = std::make_shared<Vao>(Primitives::Light::vertices, Vertex::GetLayout(), Primitives::Light::indices);

	lightVao->Bind();
		GLuint attribute = lightVao->GetLastAttribute();
		glGenBuffers(1, &transformationVbo);
		glBindBuffer(GL_ARRAY_BUFFER, transformationVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * 0, NULL, GL_STREAM_DRAW);
		glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		lightVao->AddAttribute(attribute);
		lightVao->AddVbo(transformationVbo);
	lightVao->Unbind();

	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	shader->Unbind();

  fbo = std::make_shared<LightFbo>(Window::GetSize());

	ImageAsset lightTextureAsset("Assets/Images/LightMask5.png");
	lightTexture = std::make_shared<Texture>(
		lightTextureAsset.GetSize(),
		lightTextureAsset.GetData(),
		GL_RGBA,
		lightTextureAsset.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE,
		std::vector<Texture::param_t> {
			{ ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
			{ ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
		}
	);
}

void LightPass::Execute(std::shared_ptr<ColorPass>& colorPass, std::shared_ptr<Map>& map, const Mat4& viewMatrix, const Vec2& viewPos) {
	GraphicsContext::ClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	
	bounds_t bounds = map->GetVisibleChunks(viewPos);

	if (!colorPass->light.size()) return;

  fbo->Bind();
  fbo->Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
      lightVao->Bind();
    
			glVertexAttribDivisor(2, 1);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, transformationVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * colorPass->light.size(), colorPass->light.data(), GL_STREAM_DRAW);

				lightTexture->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, lightVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr, colorPass->light.size());
				lightTexture->Unbind();

      lightVao->Unbind();
    shader->Unbind();
  fbo->Unbind();
}