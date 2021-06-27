#include "ColorPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Maths/Primitive.h"

#include "Werwel/GraphicsContext.h"

ColorPass::ColorPass(int amountOfBlocks) {
  fbo = CreateRef<ColorFBO>(Window::GetSize());

	TextAsset vsCode("Assets/Shaders/Terrain/ColorPassShader.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/ColorPassShader.fs");
	shader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_ProjectionView"
	);

	const ImageAsset tileMapTexture("Assets/Images/Map.png");
	tileMap = CreateRef<Werwel::Texture>(
		Werwel::Size(tileMapTexture.GetSize().x, tileMapTexture.GetSize().y),tileMapTexture.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT }
	);

	const auto& vers = Primitives::Block::Vertices(16, 16);
	const auto& inds = Primitives::Block::indices;

	blocks.vao = CreateRef<Werwel::VAO>();
	blocks.vao->BindSafely();
		blocks.vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
		blocks.vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);		
		blocks.vbo = blocks.vao->AddVBO(
			Werwel::VBO::Type::Array, 
			Werwel::VBO::Usage::Stream, 
			0, //amountOfBlocks, 
			sizeof(Vec4), 
			nullptr, 
			std::vector<Werwel::VertexBufferLayout> { 
				{ 4, sizeof(Vec4), 0, 1 }
			}
		);

	walls.vao = CreateRef<Werwel::VAO>();
	walls.vao->BindSafely();
		walls.vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
		walls.vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);		
		walls.vbo = walls.vao->AddVBO(
			Werwel::VBO::Type::Array, 
			Werwel::VBO::Usage::Stream, 
			0, //amountOfBlocks, 
			sizeof(BlockData), 
			nullptr, 
			std::vector<Werwel::VertexBufferLayout> { 
				{ 4, sizeof(BlockData), 0, 1 }
			}
		);
}

void ColorPass::Perform(const Ref<Camera>& camera, int amountOfWalls, int amountOfBlocks) {
	NATURAFORGE_PROFILER_SCOPE();

	Mat4 projView = Window::GetSpace() * camera->GetTransform();

	fbo->Bind();	
	fbo->Clear();
		shader->Bind();
		shader->SetMat4x4("u_ProjectionView", Math::ToPtr(projView));
			tileMap->Bind();
				walls.vao->Bind();
				walls.vao->GetIndexBuffer()->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, walls.vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfWalls);
				blocks.vao->Bind();
				blocks.vao->GetIndexBuffer()->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, blocks.vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfBlocks);

	NATURAFORGE_SYNC_GPU();
}

const Ref<Werwel::Texture>& ColorPass::GetTileMap() const {
	return tileMap;
}