#include "MapRenderer.h"

#include "Core/Time.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"
#include "Math/Primitive.h"

#include "Renderer/Vertex.h"
#include "Werwel/Texture.h"

MapRenderer::MapRenderer(Ref<Map>& map) {
	this->map = map;

	InitGraphics();

	chunks.resize(map->GetAmountOfChunks().x);

	for (int x = 0; x < map->GetAmountOfChunks().x; x++) {
		for (int y = 0; y < map->GetAmountOfChunks().y; y++) {
			bounds_t bounds = map->WhatBlocks({ x, y });
			chunks[x].emplace_back(
				Pos(x, y), 
				Size(map->GetChunkSize()),
				bounds, 
				// map->GetBlocks(),
				map->GetBlockSize()
			);
		}
	}

	Prerender();
}

void MapRenderer::Prerender() {
	for (int x = 0; x < map->GetAmountOfChunks().x; x++) {
		for (int y = 0; y < map->GetAmountOfChunks().y; y++) {
			const float currentTime = Time::GetTime();
			RerenderChunk(x, y);
			const float elapsedTime = Time::GetTime() - currentTime;
		}
	}
}

void MapRenderer::UpdateNeighborChunks(const Pos& chunkPos, const Pos& block) {
	auto& chunk = GetChunk(chunkPos);
	const auto& bounds = chunk.GetBoudns();

	bool left { false };
	bool right { false };
	bool up { false };
	bool down { false };

	if (bounds.x.start + 1 >= block.x) {
		left = true;
	} else if (bounds.x.end - 1 <= block.x) {
		right = true;
	}

	if (bounds.y.start + 1 >= block.y) {
		up = true;
	} else if (bounds.y.end - 1 <= block.y) {
		down = true;
	}

	if (left) {
		RerenderChunk(chunk.GetChunkPos().x - 1, chunk.GetChunkPos().y);
	} else if (right) {
		RerenderChunk(chunk.GetChunkPos().x + 1, chunk.GetChunkPos().y);
	}

	if (up) {
		RerenderChunk(chunk.GetChunkPos().x, chunk.GetChunkPos().y - 1);
	} else if (down) {
		RerenderChunk(chunk.GetChunkPos().x, chunk.GetChunkPos().y + 1);
	}
}

void MapRenderer::InitGraphics() {
	const ImageAsset image("Assets/Images/Map8.png");
	tileMapTexture = CreateRef<Werwel::Texture>(
		Werwel::Size(image.GetSize().x, image.GetSize().y),
		image.GetData(),
		GL_RGBA,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const auto& vertices = Primitives::Block::Vertices(map->GetBlockSize(), map->GetBlockSize());
	const auto& indices = Primitives::Block::indices;

	tileVao = CreateRef<Werwel::VAO>();
	tileVao->Bind();		
		tileVao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		tileVao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
		dynVBO = tileVao->AddVBO(
			Werwel::VBO::Type::Array, 
			Werwel::VBO::Usage::Stream, 
			map->GetChunkSize().x * map->GetChunkSize().y * 1.5f /* for walls */, sizeof(Vec4), nullptr, 
			std::vector<Werwel::VertexBufferLayout> { { 4, sizeof(Vec4), 0, 1 } }
		);
	tileVao->Unbind();

	const Vec2 halfChunkSize = (map->GetChunkSize() * map->GetBlockSize()) / 2.0f;
	const Mat4 projMatrix = Math::Ortho(-halfChunkSize.x, halfChunkSize.x, -halfChunkSize.y, halfChunkSize.y);

	const TextAsset vsCode("Assets/Shaders/Terrain/Default.vs");
	const TextAsset fsCode("Assets/Shaders/Terrain/Default.fs");
	shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View");
	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(projMatrix));
		// shader->SetVec2("u_Tile", Math::ToPtr(Vec2(1, 1)));
	shader->Unbind();
}