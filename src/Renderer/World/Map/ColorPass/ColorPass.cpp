#include "ColorPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Maths/Primitive.h"

#include "Werwel/werwel.h"

ColorPass::ColorPass(int amountOfBlocks) 
{
  	fbo = CreateRef<ColorFBO>(Window::GetSize());

	TextAsset vsCode("assets/shaders/terrain/color_pass_shader.vs");
	TextAsset fsCode("assets/shaders/terrain/color_pass_shader.fs");
	shader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_ProjectionView"
	);

	const ImageAsset tileMapTexture("assets/images/map.png");

	tileMap = TextureAtlas::Add<BlocksTileMap>(TextureAtlasType::Map, CreateRef<BlocksTileMap>(
		Vec2(8.0f),
		tileMapTexture.GetSize(),
		tileMapTexture.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		Werwel::Texture::Parameters_t {
			Werwel::Texture::SetInterpolation(Werwel::Interpolation::Constant)
		}
	));
	tileMap->Add(BlockType::Dirt, Vec2(1, 1));
	tileMap->Add(BlockType::Grass, Vec2(1, 7));
	tileMap->Add(BlockType::Stone, Vec2(7, 1));
	tileMap->Add(BlockType::Wood, Vec2(13, 1));

	const auto& vers = Primitives::Block::Vertices(16, 16);
	const auto& inds = Primitives::Block::indices;

	blocks.vao = CreateRef<Werwel::VAO>();
	blocks.vao->Bind();
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
	walls.vao->Bind();
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

	Window::callbacks.push_back([&]() {
		fbo->Resize(Window::GetSize());
	});
}

void ColorPass::Perform(const Ref<Camera>& camera, int amountOfWalls, int amountOfBlocks, const std::vector<Ref<IRenderer>>& additionalRenderers) 
{
	NF_PROFILER_SCOPE();

	Mat4 projView = Window::GetSpace() * camera->GetTransform();

	fbo->Bind();	
	fbo->Clear();

		shader->Bind();
		shader->SetMat4x4("u_ProjectionView", Math::ToPtr(projView));
			tileMap->Bind();
				walls.vao->Bind();
				walls.vao->GetIndexBuffer()->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, walls.vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfWalls);

		for (const auto& renderer : additionalRenderers) 
		{
			renderer->Render();
		}

		shader->Bind();
			tileMap->Bind();
				blocks.vao->Bind();
				blocks.vao->GetIndexBuffer()->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, blocks.vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfBlocks);

	NF_SYNC_GPU();
}