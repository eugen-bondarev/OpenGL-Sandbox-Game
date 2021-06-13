#pragma once

#include "Core/Window.h"

#include "Blocks.h"
#include "Types.h"

struct BlockSettingData {
	Pos block { -1, -1 };
	Pos chunk { -1, -1 };

	inline bool IsSet() const {
		return chunk != Pos { -1, -1 };
	}
};

struct MapChunk {
	Pos index;

	int memPos;

	bool shown { false };
};

class Map {
public:
	std::vector<
		std::vector<MapChunk>
	> chunks;

  Map(Size chunkSize, Size amountOfChunks, float blockSize = 16.0f);

	BlockSettingData SetBlock(const Mat4& viewMatrix, BlockType blockType);

	inline Pos WhatChunk(Pos block) const {
		int x = static_cast<int>(truncf(block.x / GetChunkSize().x));
		int y = static_cast<int>(truncf(block.y / GetChunkSize().y));

		return { x, y };
	}

	inline chunk_t WhatBlocks(Pos chunk) const {
		Period<> x { static_cast<int>(chunk.x * GetChunkSize().x), static_cast<int>((chunk.x + 1.0f) * GetChunkSize().x) };
		Period<> y { static_cast<int>(chunk.y * GetChunkSize().y), static_cast<int>((chunk.y + 1.0f) * GetChunkSize().y) };
		return { x, y };
	}

	inline Vec2 WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4& projectionMatrix, const Mat4& viewMatrix) const {
		const Vec2& viewPos = viewMatrix[0];

		const Vec2 screenCoords = (windowCoords / Window::GetSize() - Vec2(0.5f, 0.5f)) * Vec2(1.0f, -1.0f) * 2.0f;
		const Vec4 projCoords = Math::Inverse(projectionMatrix) * Vec4(screenCoords, 0.0f, 1.0f);
		const Vec4 projViewCoords = Math::Inverse(viewMatrix) * projCoords;

		static const Vec2 normalization = Vec2(0.0f);
		const Vec2 block = (Vec2(projViewCoords) - viewPos) / blockSize + normalization + GetChunkSize() / 2.0f;
		
		return block;
	}

  inline Size GetChunkSize() const {
    return chunkSize;
  }

  inline Size GetAmountOfChunks() const {
    return amountOfChunks;
  }

	inline Pos GetCenter() const {
		return amountOfBlocks / 2.0f;
	}

	void CalculateVisibleChunks(Pos viewPos);
	
	inline const bounds_t& GetVisibleChunks() const {
		return visibleChunks;
	}

	inline float GetBlockSize() const {
		return blockSize;
	}

	inline const blocks_t& GetBlocks() const {
		return blocks;
	}

	inline const walls_t& GetWalls() const {
		return walls;
	}

	inline bool BlockIs(int x, int y, BlockType type) const {
		return blocks[x][y] == type;
	}

	inline bool WallIs(int x, int y, WallType type) const {
		return walls[x][y] == type;
	}

private:
	blocks_t blocks;
	walls_t walls;

  Size chunkSize;
  Size amountOfChunks;
	Size amountOfBlocks;

	float blockSize;

	void GenerateMap();

	bounds_t visibleChunks;
};