#include "map.h"

#include "renderer/world/map/tiles.h"
#include "renderer/atlas/texture_atlas.h"

#include "FastNoise/FastNoiseLite.h"

namespace MapSettings 
{
	float SIZE_0 = 0.3f;
	float SIZE_1 = 0.001f;
	float SIZE_2 = 0.1f;
	float BIAS_0 = 0.0f;
	float BIAS_1 = 0.0f;
}

namespace Map
{
	FastNoiseLite Noise;

	Blocks_t Blocks;
	Walls_t Walls;
	PlacedBlocks_t PlacedBlocks;

	Bounds_t VisibleChunks;
	Bounds_t LastVisibleChunks;

	MapFlags_ Flags = MapFlags_None;
	
	float WhatNoise(float x, float y)
	{
		return Noise.GetNoise(x * MapSettings::SIZE_0, y * MapSettings::SIZE_0) * 0.5f + 0.5f;
	}

	BlockType WhatBlockType(float noise_value, TilePos tile_pos, float x, float y)
	{
		BlockType type = BlockType::Empty;

		if (tile_pos == TilePos::Foreground)
		{
			if (noise_value + MapSettings::BIAS_0 > 0.75f) 
				type = BlockType::Empty;

			if (noise_value + MapSettings::BIAS_0 >= 0.3f && noise_value < 0.75f)
				type = BlockType::Grass;

			if (noise_value + MapSettings::BIAS_0 < 0.3f)
				type = BlockType::Stone;
		}
		else
		{
			if (noise_value + MapSettings::BIAS_1 > 0.75f) 
				type = BlockType::Grass;

			if (noise_value + MapSettings::BIAS_1 >= 0.3f && noise_value < 0.75f)
				type = BlockType::Grass;

			if (noise_value + MapSettings::BIAS_1 < 0.3f)
				type = BlockType::Stone;
		}

		return type;
	}

	BlockRepresentation WhatBlock(float noise_value, TilePos tile_pos, float x, float y)
	{
		BlockRepresentation representation;

		representation.type = WhatBlockType(noise_value, tile_pos, x, y);
		representation.position = Vec2(x, y);
		
		Vec2 tile;

		if (representation.type == BlockType::Empty)
			representation.tile = Vec2(2, 4);
		
		if (representation.type == BlockType::Grass)
			representation.tile = Vec2(1, 1);
		
		if (representation.type == BlockType::Stone)
			representation.tile = Vec2(7, 1);

		return representation;
	}

	void PopulateVisibleMap()
	{
		MW_PROFILER_SCOPE();

		static int height_variation = 5000;
		static int horizon = 0;

		Vec2 chunk_indices;
		Vec2 block_indices;

		for (chunk_indices.x = VisibleChunks.x.start; chunk_indices.x < VisibleChunks.x.end; ++chunk_indices.x)
		{
			for (chunk_indices.y = VisibleChunks.y.start; chunk_indices.y < VisibleChunks.y.end; ++chunk_indices.y)
			{
				PlacedBlocks_t::iterator chunk_iterator = PlacedBlocks.find(chunk_indices);

				for (block_indices.x = 0; block_indices.x < CHUNK_SIZE; ++block_indices.x)
				{
					Vec2 block_pos;
					block_pos.x = (chunk_indices.x * CHUNK_SIZE + block_indices.x) * BLOCK_SIZE;				

					int height_in_this_area = height_variation * Noise.GetNoise(block_pos.x * MapSettings::SIZE_1, 0.0f);

					for (block_indices.y = 0; block_indices.y < CHUNK_SIZE; ++block_indices.y)
					{
						block_pos.y = (chunk_indices.y * CHUNK_SIZE + block_indices.y) * BLOCK_SIZE;

						Vec2 visible_chunks_start = Vec2(VisibleChunks.x.start, VisibleChunks.y.start);
						Vec2 indices = trunc(chunk_indices - visible_chunks_start) * CHUNK_SIZE + block_indices;
							
						// I don't know why this case occures.
						if (indices.y >= Blocks[0].size() || indices.x >= Blocks.size())
							continue;

						if (chunk_iterator != PlacedBlocks.end())
						{
							PlacedBlocksInChunk_t::iterator block_iterator = chunk_iterator->second.find(block_indices);
							if (block_iterator != chunk_iterator->second.end())
							{
								Blocks[indices.x][indices.y].type = block_iterator->second;
								Blocks[indices.x][indices.y].worldPosition = block_pos;

								continue;
							}
						}

						if (block_pos.y > horizon * CHUNK_SIZE + height_in_this_area * Noise.GetNoise(block_pos.x * MapSettings::SIZE_2, 0.0f))
						{
							Blocks[indices.x][indices.y].type = BlockType::Empty;
							Blocks[indices.x][indices.y].worldPosition = block_pos;

							Walls[indices.x][indices.y].type = BlockType::Empty;
							Walls[indices.x][indices.y].worldPosition = block_pos;
						}
						else
						{
							float noiseValue = WhatNoise(block_pos.x, block_pos.y);

							BlockRepresentation computedBlock = WhatBlock(noiseValue, TilePos::Foreground, block_pos.x, block_pos.y);
							BlockRepresentation computedWall = WhatBlock(noiseValue, TilePos::Background, block_pos.x, block_pos.y);

							Blocks[indices.x][indices.y].type = computedBlock.type;
							Blocks[indices.x][indices.y].worldPosition = computedBlock.position;

							Walls[indices.x][indices.y].type = computedWall.type;
							Walls[indices.x][indices.y].worldPosition = computedWall.position;
						}
					}
				}
			}
		}

		MW_SYNC_GPU();
	}

	void Init(int seed)
	{
		auto& chunk = PlacedBlocks[{ 0, 0 }];
		auto& block = chunk[{ 0, 0 }];
		block = BlockType::Wood;
		
		// Noise.SetNoiseType(FastNoiseLite::NoiseType_Value);
		Noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		Noise.SetSeed(seed);
	}

	void CheckVisibleChunks()
	{	
		if (VisibleChunks != LastVisibleChunks)
		{
			PopulateVisibleMap();
			LastVisibleChunks = VisibleChunks;
			Flags |= MapFlags_ChunksUpdated;
		}
	}

	void CalculateVisibleChunks(Vec2 view_pos)
	{	
		Vec2 correction = Vec2(-16, 16);

		VisibleChunks.x.start = (view_pos.x + correction.x - MW_WINDOW_WIDTH() / 2.0f) / 16.0f / CHUNK_SIZE;
		VisibleChunks.x.end = VisibleChunks.x.start + MW_WINDOW_WIDTH() / (CHUNK_SIZE * BLOCK_SIZE) + 2;
		VisibleChunks.y.start = (view_pos.y + correction.y - MW_WINDOW_HEIGHT() / 2.0f) / 16.0f / CHUNK_SIZE;
		VisibleChunks.y.end = VisibleChunks.y.start + MW_WINDOW_HEIGHT() / (CHUNK_SIZE * BLOCK_SIZE) + 2;
		VisibleChunks.x.start -= 1;
		VisibleChunks.y.start -= 2;
	}

	Blocks_t &GetBlocks()
	{
		return Blocks;
	}

	Walls_t &GetWalls()
	{
		return Walls;
	}

	bool BlockIs(int x, int y, BlockType type)
	{
		return Blocks[x][y].type == type;
	}

	bool WallIs(int x, int y, WallType type)
	{
		return Walls[x][y].type == type;
	}

	bool BlockIsEmpty(int x, int y)
	{
		return BlockIs(x, y, BlockType::Empty);
	}

	bool WallIsEmpty(int x, int y)
	{
		return WallIs(x, y, WallType::Empty);
	}

	bool InBounds(int x, int y)
	{
		return x >= 0 && y >= 0 && x < Blocks.size() && y < Blocks[0].size();
	}

	Bounds_t& GetVisibleChunks()
	{
		return VisibleChunks;
	}

	Bounds_t& GetLastVisibleChunks()
	{
		return LastVisibleChunks;
	}
}
