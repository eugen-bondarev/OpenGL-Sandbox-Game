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
		Period<> x{ chunk.x * chunkSize.x, (chunk.x + 1) * chunkSize.x };
		Period<> y{ chunk.y * chunkSize.y, (chunk.y + 1) * chunkSize.y };
		return { x, y };
	}

private:
	blocks_t blocks;

	Size chunkSize;
	Size amountOfChunks;
	Size amountOfBlocks;

	void GenerateMap();
};