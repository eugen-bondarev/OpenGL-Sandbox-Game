#pragma once

#include "Core/Window.h"

#include "Blocks.h"
#include "Types.h"

class Map {
public:
  Map(Vec2 chunkSize, Vec2 amountOfChunks, float blockSize = 16.0f);

	struct BlockSettingData {
		Vec2 block { -1, -1 };
		Vec2 chunk { -1, -1 };

		inline bool IsSet() const {
			return chunk != Vec2 { -1, -1 };
		}
	};

	BlockSettingData SetBlock(const Vec2& viewMatrix, BlockType blockType);

	Vec2 WhatChunk(Vec2 block) const;
	chunk_t WhatBlocks(Vec2 chunk) const;
	
	Vec2 WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4& projectionMatrix, const Mat4& viewMatrix) const;

  Vec2 GetChunkSize() const;
  Vec2 GetAmountOfChunks() const;
	Vec2 GetCenter() const;

	void CalculateVisibleChunks(Vec2 viewPos);	

	const bounds_t& GetVisibleChunks() const;
	float GetBlockSize() const;

	blocks_t& GetBlocks();
	const walls_t& GetWalls() const;

	bool BlockIs(int x, int y, BlockType type) const;
	bool WallIs(int x, int y, WallType type) const;
  bool BlockIsEmpty(int x, int y) const;
  bool WallIsEmpty(int x, int y) const;

private:
	blocks_t blocks;
	walls_t walls;

  Vec2 chunkSize;
  Vec2 amountOfChunks;
	Vec2 amountOfBlocks;

	float blockSize;

	void GenerateMap();

	bounds_t visibleChunks;
};