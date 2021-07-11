#pragma once

#include "blocks.h"
#include "types.h"

namespace MapSettings {

extern float SIZE_0;
extern float SIZE_1;
extern float SIZE_2;
extern float BIAS_0;
extern float BIAS_1;

}

// The specialized hash function for `unordered_map` keys
struct hash_fn
{
    std::size_t operator()(const Vec2 &node) const
    {
        std::size_t h1 = std::hash<float>()(node.x);
        std::size_t h2 = std::hash<float>()(node.y);
 
        return h1 ^ h2;
    }
};

enum MapFlags_
{
	MapFlags_None = 0,
	MapFlags_BlocksUpdated = 1 << 0,
	MapFlags_ChunksUpdated = 1 << 1,
};

inline MapFlags_& operator |=(MapFlags_& a, MapFlags_ b)
{
	return a = static_cast<MapFlags_>(static_cast<int>(a) | static_cast<int>(b));
}

namespace Map
{
	static constexpr float BLOCK_SIZE = 16.0f;

	using PlacedBlocksInChunk_t = std::unordered_map<Vec2, BlockType, hash_fn>;  // Indexed by (relative) block's index.
	using PlacedBlocks_t = std::unordered_map<Vec2, PlacedBlocksInChunk_t, hash_fn>;  // Indexed by absolute chunk's index.

	using MapFlags_t = int;

	extern Blocks_t Blocks;
	extern Walls_t Walls;
	extern PlacedBlocks_t PlacedBlocks;

	extern Vec2 ChunkSize;

	extern Bounds_t VisibleChunks;
	extern Bounds_t LastVisibleChunks;

	extern MapFlags_ Flags;

	void Init(int seed);

	void PopulateVisibleMap();

	void CheckVisibleChunks();
	void CalculateVisibleChunks(Vec2 view_pos);
	Vec2 GetChunkSize();

	Blocks_t &GetBlocks();
	Walls_t &GetWalls();

	bool BlockIs(int x, int y, BlockType type);
	bool WallIs(int x, int y, WallType type);
	bool BlockIsEmpty(int x, int y);
	bool WallIsEmpty(int x, int y);
}

using TileToUpdate = Vec2;

struct BlockRepresentation
{
	BlockType type;
	Vec2 position;
	Vec2 tile;
};

BlockType WhatBlockType(float noiseValue, TilePos tilePos, float x, float y);
BlockRepresentation WhatBlock(float noiseValue, TilePos tilePos, float x, float y);