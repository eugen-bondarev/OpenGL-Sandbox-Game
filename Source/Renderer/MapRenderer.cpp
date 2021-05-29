#include "MapRenderer.h"

#include "Core/Time.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"
#include "Gpu/Vertex.h"
#include "Math/Primitive.h"

MapRenderer::MapRenderer(std::shared_ptr<Map>& map) {
	this->map = map;

	InitGraphics();

	chunks.resize(map->GetAmountOfChunks().x);
	for (int x = 0; x < map->GetAmountOfChunks().x; x++) {
		for (int y = 0; y < map->GetAmountOfChunks().y; y++) {
			bounds_t bounds = map->WhatBlocks({ x, y });
			chunks[x].emplace_back(
				Pos {x, y}, 
				Size {map->GetChunkSize()}, 
				shader,
				tileVao, 
				tileMapTexture, 
				bounds, 
				map->blocks
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

void MapRenderer::InitGraphics() {
	ImageAsset image("Assets/Images/Map2.png");
	tileMapTexture = std::make_shared<Texture>(
		image.GetSize(),
		image.GetData(),
		GL_RGBA,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE,
		std::vector<Texture::param_t> {
			{ ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
			{ ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
		}
	);

	tileVao = std::make_shared<Vao>(
		Primitives::Quad::vertices,
		Vertex::GetLayout(),
		Primitives::Quad::indices
	);

	Vec2 halfChunkSize = (map->GetChunkSize() * BLOCK_SIZE) / 2.0f;
	Mat4 projMatrix = Math::Ortho(-halfChunkSize.x, halfChunkSize.x, -halfChunkSize.y, halfChunkSize.y);

	TextAsset vsCode("Assets/Shaders/Terrain/Default.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/Default.fs");
	shader = std::make_shared<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_Tile");
	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(projMatrix));
		shader->SetVec2("u_Tile", Math::ToPtr(Vec2(1, 1)));
	shader->Unbind();
}