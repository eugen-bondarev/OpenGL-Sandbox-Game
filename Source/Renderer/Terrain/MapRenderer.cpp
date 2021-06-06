#include "MapRenderer.h"

#include "Core/Time.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"
#include "Math/Primitive.h"

#include "GPU/Vertex.h"
#include "GPU/Texture.h"

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
				shader,
				tileVao, 
				tileMapTexture, 
				bounds, 
				map->blocks,
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
			chunks[x][y].Rerender();
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
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x - 1][chunk.GetChunkPos().y];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	} else if (right) {
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x + 1][chunk.GetChunkPos().y];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	}

	if (up) {
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x][chunk.GetChunkPos().y - 1];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	} else if (down) {
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x][chunk.GetChunkPos().y + 1];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	}
}

void MapRenderer::InitGraphics() {
	const ImageAsset image("Assets/Images/Map7.png");
	tileMapTexture = CreateRef<Texture>(
		image.GetSize(),
		image.GetData(),
		GL_RGBA,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE,
		Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const auto& vertices = Primitives::Block::Vertices(map->GetBlockSize(), map->GetBlockSize());
	const auto& indices = Primitives::Block::indices;

	tileVao = CreateRef<VAO>();
	tileVao->Bind();		
		tileVao->AddVBO(VBO::Type::Array, VBO::Usage::Static, vertices.size(), sizeof(Vertex), &vertices[0], Vertex::GetLayout());
		tileVao->AddVBO(VBO::Type::Indices, VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
	tileVao->Unbind();

	const Vec2 halfChunkSize = (map->GetChunkSize() * map->GetBlockSize()) / 2.0f;
	const Mat4 projMatrix = Math::Ortho(-halfChunkSize.x, halfChunkSize.x, -halfChunkSize.y, halfChunkSize.y);

	const TextAsset vsCode("Assets/Shaders/Terrain/Default.vs");
	const TextAsset fsCode("Assets/Shaders/Terrain/Default.fs");
	shader = CreateRef<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_Tile");
	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(projMatrix));
		shader->SetVec2("u_Tile", Math::ToPtr(Vec2(1, 1)));
	shader->Unbind();
}