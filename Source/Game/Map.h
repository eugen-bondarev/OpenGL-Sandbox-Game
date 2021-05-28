#pragma once

#include "Blocks.h"

#include "Util/Structures.h"

#include "Chunk.h"

#include "Gpu/ChunkFbo.h"
#include "Gpu/Vao.h"
#include "Gpu/Texture.h"
#include "Gpu/Shader.h"

#include "Math/Math.h"

#include "Types.h"

class Map {
public:
	Map(Size chunkSize, Size amountOfChunks);

	using chunks_row_t = std::vector<Chunk>;
	using chunks_t = std::vector<chunks_row_t>;

	inline Pos WhatChunk(Pos block) const {
		int x = static_cast<int>(truncf(block.x / chunkSize.x));
		int y = static_cast<int>(truncf(block.y / chunkSize.y));

		return { x, y };
	}

	inline chunk_t WhatBlocks(Pos chunk) const {
		Period<> x{ static_cast<int>(chunk.x * chunkSize.x), static_cast<int>((chunk.x + 1.0f) * chunkSize.x) };
		Period<> y{ static_cast<int>(chunk.y * chunkSize.y), static_cast<int>((chunk.y + 1.0f) * chunkSize.y) };
		return { x, y };
	}

	blocks_t blocks;
	chunks_t chunks;

	inline Pos GetCenter() const {
		return amountOfBlocks / 2.0f;
	}

	inline Size GetChunkSize() const {
		return chunkSize;
	}

	ChunkFbo* chunkFbo { nullptr };

	void InitGraphics();
	std::shared_ptr<Vao> tileVao;
	std::shared_ptr<Texture> tileMap;
	std::shared_ptr<Shader> shader;

	Mat4 viewMatrix{Mat4(1)};
	Vec2 viewPos{Vec2(0)};

	Mat4 projMatrix{Mat4(1)};

private:
	Size chunkSize;
	Size amountOfChunks;
	Size amountOfBlocks;

	void GenerateMap();
};