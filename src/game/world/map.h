#pragma once

#include "blocks.h"
#include "types.h"

struct MapGenerationDataSet
{
	int maxHeight = 50;
	int maxDepth = 25;
	int maxSlopeMistake = 2;

	int maxLength = 200;
	int minLength = 15;

	int blockProbabilityInPercent = 10;

	int seed = 669;
};

inline static constexpr MapGenerationDataSet DEFAULT_DATA_SET = {};

using TileToUpdate = Vec2;

using Row = std::vector<Tile>;
using Chunk = std::vector<Row>;

using RowOfChunks = std::vector<Chunk>;
using Chunks = std::vector<RowOfChunks>;

extern Vec2 START_POS;

class Map;

void ConvertChunksRenderData(Map* map, Vec2 startWorldCoords, Chunks& chunks, std::vector<Vec4>& data);

Vec4 WhatBlock(float x, float y);

struct ChunkData
{
	int start { 0 };
	int howMany { 0 };
};

struct Compare final
{
	bool operator()(const Vec2& lhs, const Vec2& rhs) const noexcept
	{
		return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
	}
};

extern std::map<Vec2, ChunkData, Compare> chunkData;
extern Chunks chunks;
extern std::vector<Vec4> renderData;

class Map
{
public:

	Map(int seed, Vec2 chunkSize, Vec2 amountOfChunks, float blockSize = 16.0f);

	struct BlockSettingData
	{
		Vec2 block{-1, -1};
		Vec2 chunk{-1, -1};

		BlockType blockType;
		BlockType oldBlock;

		inline bool IsSet() const
		{
			return chunk != Vec2{-1, -1};
		}
	};

	TileType GetTileUnderCursor(const Vec2 &cameraPosition, const tiles_t &tiles) const;
	BlockType GetBlockUnderCursor(const Vec2 &cameraPosition) const;
	WallType GetWallUnderCursor(const Vec2 &cameraPosition) const;

	BlockSettingData Place(const Vec2 &cameraPosition, BlockType blockType, TilePos tilePos);
	BlockSettingData PlaceBlock(const Vec2 &cameraPosition, BlockType blockType);
	BlockSettingData PlaceWall(const Vec2 &cameraPosition, WallType wallType);

	bool HasNeighbor(int x, int y, BlockType block) const;
	bool HasEmptyNeighbor(int x, int y) const;

	bool CheckBounds(int x, int y) const;
	void SetBlock(int x, int y, BlockType type);

	Vec2 WhatChunk(Vec2 block) const;
	chunk_t WhatBlocks(Vec2 chunk) const;

	Vec2 WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4 &projectionMatrix, const Mat4 &viewMatrix) const;

	Vec2 GetChunkSize() const;
	Vec2 GetAmountOfChunks() const;
	Vec2 GetCenter() const;

	void CalculateVisibleChunks(Vec2 viewPos);

	bounds_t &GetVisibleChunks();
	bounds_t &GetLastVisibleChunks();

	float GetBlockSize() const;

	blocks_t &GetBlocks();
	walls_t &GetWalls();

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

	bool blocksUpdated{true};
	bool chunksUpdated{true};

	TileToUpdate blockToUpdate{-1};
	TileToUpdate wallToUpdate{-1};

private:
	blocks_t blocks;
	walls_t walls;

	Vec2 chunkSize;
	Vec2 amountOfChunks;
	Vec2 amountOfBlocks;

	float blockSize;

	void GenerateMap(MapGenerationDataSet generationDataSet);

	bounds_t visibleChunks;
	bounds_t lastVisibleChunks;
};