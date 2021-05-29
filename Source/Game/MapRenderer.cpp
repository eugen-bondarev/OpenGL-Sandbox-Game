#include "MapRenderer.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"
#include "Gpu/Vertex.h"
#include "Math/Primitive.h"

void MapRenderer::InitGraphics() {
	ImageAsset image("Assets/Images/Map2.png");
	graphics.tileMapTexture = std::make_shared<Texture>(
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

	graphics.tileVao = std::make_shared<Vao>(
		Primitives::Quad::vertices,
		Vertex::GetLayout(),
		Primitives::Quad::indices
	);

	TextAsset vsCode("Assets/Shaders/Terrain/Default.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/Default.fs");

	Vec2 halfChunkSize = (chunkSize * BLOCK_SIZE) / 2.0f;
	Mat4 projMatrix = Math::Ortho(-halfChunkSize.x, halfChunkSize.x, -halfChunkSize.y, halfChunkSize.y);

	graphics.shader = std::make_shared<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_Tile");
	graphics.shader->Bind();
		graphics.shader->SetMat4x4("u_Proj", Math::ToPtr(projMatrix));
		graphics.shader->SetVec2("u_Tile", Math::ToPtr(Vec2(1, 1)));
	graphics.shader->Unbind();
}

MapRenderer::MapRenderer(Size chunkSize, Size amountOfChunks) {
	this->chunkSize = chunkSize;
	this->amountOfChunks = amountOfChunks;

	InitGraphics();

	chunks.resize(amountOfChunks.x);
	for (int x = 0; x < amountOfChunks.x; x++) {
		for (int y = 0; y < amountOfChunks.y; y++) {
			bounds_t bounds = WhatBlocks({ x, y });
			chunks[x].emplace_back(
				Pos {x, y}, 
				Size {chunkSize}, 
				graphics.shader, 
				graphics.tileVao, 
				graphics.tileMapTexture, 
				bounds, 
				blocks
			);
		}
	}

	GenerateMap();
}

void MapRenderer::GenerateMap() {
	amountOfBlocks = chunkSize * amountOfChunks;
	const int amountOfColumns = amountOfBlocks.x;
	const int amountOfRows = amountOfBlocks.y;

	blocks.resize(amountOfColumns);
	for (int x = 0; x < amountOfColumns; x++) {
		blocks[x].resize(amountOfRows);
	}

	const int middle = static_cast<int>(amountOfBlocks.y / 2.0f);

	for (int x = 0; x < amountOfBlocks.x; x++) {
		for (int y = 0; y < middle; y++) {
			blocks[x][y] = BlockType::Dirt;
		}
		
		blocks[x][middle] = BlockType::Grass;

		for (int y = middle + 1; y < amountOfBlocks.y; y++) {
			blocks[x][y] = BlockType::Empty;
		}
	}
}