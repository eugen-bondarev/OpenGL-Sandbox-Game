#include "Map.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"
#include "Gpu/Vertex.h"
#include "Math/Primitive.h"

void Map::InitGraphics() {
	ImageAsset image("Assets/Images/Map2.png");
	tileMap = std::make_shared<Texture>(
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

	TextAsset vsCode("Assets/Shaders/Terrain/Default.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/Default.fs");

	// viewPos = map->GetCenter() * 16.0f;
	viewPos = (Vec2(1, 0) * 12.0f) * 16.0f;
	viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));
	// viewMatrix = Mat4(1);

	float chunkSize = 192.0f;
	float halfChunkSize = chunkSize / 2.0f;
	projMatrix = Math::Ortho(-halfChunkSize, halfChunkSize, -halfChunkSize, halfChunkSize);

	shader = std::make_shared<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_Tile");
	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(projMatrix));
		shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
		shader->SetVec2("u_Tile", Math::ToPtr(Vec2(1, 1)));
	shader->Unbind();
}

Map::Map(Size chunkSize, Size amountOfChunks) {
	this->chunkSize = chunkSize;
	this->amountOfChunks = amountOfChunks;

	// chunkFbo = std::make_shared<ChunkFbo>(Size{ 192, 192 });
	// chunkFbo = new ChunkFbo({ 192, 192 });
	InitGraphics();

	chunks.resize(amountOfChunks.x);
	for (int x = 0; x < amountOfChunks.x; x++) {
		for (int y = 0; y < amountOfChunks.y; y++) {
			auto chunk = WhatBlocks({ x, y });
			chunks[x].emplace_back(Pos {x, y}, Size {chunkSize}, &chunkFbo, shader.get(), tileVao.get(), tileMap.get(), chunk, &blocks);
		}
		// chunks[x].resize(amountOfChunks.y);
	}

	amountOfBlocks = chunkSize * amountOfChunks;
	GenerateMap();
}

void Map::GenerateMap() {
	const int amountOfColumns = amountOfBlocks.x;
	const int amountOfRows = amountOfBlocks.y;

	blocks.resize(amountOfColumns);
	for (int x = 0; x < amountOfColumns; x++) {
		blocks[x].resize(amountOfRows);
	}

	int middle = static_cast<int>(amountOfBlocks.y / 2.0f);

	for (int x = 0; x < amountOfBlocks.x; x++) {
		for (int y = 0; y < middle; y++) {
			blocks[x][y] = BlockType::Dirt;
		}

		for (int y = middle; y < amountOfBlocks.y; y++) {
			blocks[x][y] = BlockType::Empty;
		}
	}
}