#include "ColorPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Maths/Primitive.h"

ColorPass::ColorPass(int amountOfBlocks) {
  fbo = CreateRef<ColorFBO>(Window::GetSize());

	TextAsset vsCode("Assets/Shaders/Terrain/ColorPassShader.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/ColorPassShader.fs");
	shader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_Proj", "u_View"
	);

	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));

	const ImageAsset tileMapTexture("Assets/Images/Map.png");
	tileMap = CreateRef<Werwel::Texture>(
		Werwel::Size(tileMapTexture.GetSize().x, tileMapTexture.GetSize().y),
		tileMapTexture.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const auto& vers = Primitives::Block::Vertices(16, 16);
	const auto& inds = Primitives::Block::indices;

	vao = CreateRef<Werwel::VAO>();
	vao->BindSafely();
		vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
		vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);
		vbo = vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Stream, amountOfBlocks, sizeof(Vec4), nullptr, std::vector<Werwel::VertexBufferLayout> { { 4, sizeof(Vec4), 0, 1 } });
}

const Ref<ColorFBO>& ColorPass::GetFBO() const {
  return fbo;
}