#pragma once

#include "Blocks.h"

#include "Util/Structures.h"

class Map {
public:
	Map(Size chunkSize, Size amountOfChunks);

	using row_t = std::vector<BlockType>;
	using blocks_t = std::vector<row_t>;
	using chunk_t = struct {
		Period<> x;
		Period<> y;
	};

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

	inline Pos GetCenter() const {
		return amountOfBlocks / 2.0f;
	}

private:
	Size chunkSize;
	Size amountOfChunks;
	Size amountOfBlocks;

	void GenerateMap();
};