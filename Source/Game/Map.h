#pragma once

#include "Core/Window.h"

#include "Blocks.h"
#include "Types.h"

struct MapGenerationDataSet {
	int maxHeight = 50;
	int maxDepth = 25;
	int maxSlopeMistake = 2;	

	int maxLength = 200;
	int minLength = 15;

	int blockProbabilityInPercent = 10;

	int seed = 669;
};

inline static constexpr MapGenerationDataSet DEFAULT_DATA_SET = {};

class Map {
public:
  Map(int seed, Vec2 chunkSize, Vec2 amountOfChunks, float blockSize = 16.0f);

	struct BlockSettingData {
		Vec2 block { -1, -1 };
		Vec2 chunk { -1, -1 };

		inline bool IsSet() const {
			return chunk != Vec2 { -1, -1 };
		}
	};

	BlockSettingData PlaceBlock(const Vec2& viewMatrix, BlockType blockType);

	bool HasNeighbor(int x, int y, BlockType block) const;
	bool HasEmptyNeighbor(int x, int y) const;

	bool CheckBounds(int x, int y) const;
	void SetBlock(int x, int y, BlockType type);

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

	int GetWidth() const;
	int GetHeight() const;
	int GetArea() const;
	int GetSizeInBytes() const;
	int GetSizeInKilobytes() const;
	int GetSizeInMegabytes() const;

private:
	blocks_t blocks;
	walls_t walls;

  Vec2 chunkSize;
  Vec2 amountOfChunks;
	Vec2 amountOfBlocks;

	float blockSize;

	void GenerateMap(MapGenerationDataSet generationDataSet);

	bounds_t visibleChunks;
};