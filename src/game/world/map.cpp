#include "map.h"

#include "FastNoise/FastNoiseLite.h"

#include "renderer/world/map/tiles.h"

#include "renderer/atlas/texture_atlas.h"

FastNoiseLite noise1;
std::map<Vec2, ChunkData, Compare> chunkData;

std::vector<Vec4> renderData;

std::map<Vec2, ChunkData, Compare> light_associations;
std::vector<Vec2> light_data;

BlockType WhatBlockType(float x, float y)
{
	BlockType type;

	static float size = 0.5f;
	float value = noise1.GetNoise(x * size, y * size) * 0.5f + 0.5f;

	if (y > 32600 + 1000 * noise1.GetNoise(x * 0.1f, 0.0f))
	{
		value = 1.0f;
	}
	
	if (value > 0.75f) 
	{
		type = BlockType::Empty;
	}
	if (value >= 0.3f && value < 0.75f)
	{
		type = BlockType::Grass;
	}
	if (value < 0.3f)
	{
		type = BlockType::Stone;
	}

	return type;
}

BlockRepresentation WhatBlock(float x, float y)
{
	BlockRepresentation representation;

	representation.type = WhatBlockType(x, y);

	representation.position = Vec2(x, y);
	
	Vec2 tile;
	if (representation.type == BlockType::Empty)
	{
		representation.tile = Vec2(2, 4);
	}
	
	if (representation.type == BlockType::Grass)
	{
		representation.tile = Vec2(1, 1);
	}
	
	if (representation.type == BlockType::Stone)
	{
		representation.tile = Vec2(7, 1);
	}

	return representation;
}

void ConvertChunksRenderData(Map* map, std::vector<Vec4>& data, std::vector<Vec2>& l_data)
{
	// CheckScope timer("ConvertChunksRenderData");

	BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);
	
	noise1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

	int i = 0;

	auto& visibleChunks = map->GetVisibleChunks();
	data.resize(visibleChunks.GetArea() * map->GetChunkSize().x * map->GetChunkSize().y);
	l_data.resize(visibleChunks.GetArea() * map->GetChunkSize().x * map->GetChunkSize().y);

	for (int xChunk = visibleChunks.x.start; xChunk < visibleChunks.x.end; xChunk++)
	{
		for (int yChunk = visibleChunks.y.start; yChunk < visibleChunks.y.end; yChunk++)
		{
			Vec2 chunkIndices = Vec2(xChunk, yChunk);

			Vec2 chunkPosition = chunkIndices;

			int start = i;

			for (int x = 0; x < map->GetChunkSize().x; x++)
			{
				for (int y = 0; y < map->GetChunkSize().y; y++)
				{
					Vec2 blockPosition = (chunkPosition * map->GetChunkSize() + Vec2(x, y)) * 16.0f;
					
					BlockRepresentation representation = WhatBlock(blockPosition.x, blockPosition.y);

					int xi = static_cast<int>(truncf((xChunk - visibleChunks.x.start) * 8.0f + x));
					int yi = static_cast<int>(truncf((yChunk - visibleChunks.y.start) * 8.0f + y));
					map->BLOCKS[xi][yi].type = representation.type;
					map->BLOCKS[xi][yi].texture = representation.position;

					data[i] = Vec4(representation.position.x, representation.position.y, representation.tile.x, representation.tile.y);
					l_data[i] = representation.type == BlockType::Empty ? representation.position : Vec2(0);
					i++;
				}
			}

			int howMany = i - start;

			Vec2 ind = chunkIndices;
			ind = trunc(ind);

			chunkData[ind] = { start, howMany };
		}
	}
}

Map::Map(int seed, Vec2 chunkSize, Vec2 amountOfChunks, float blockSize) : chunkSize{chunkSize}, amountOfChunks{amountOfChunks}, blockSize{blockSize}
{
	MapGenerationDataSet mapGenerator = {};
	mapGenerator.seed = seed;
	GenerateMap(mapGenerator);
	noise1.SetSeed(seed);

	Vec2 AMOUNT_OF_CHUNKS = ceil(MW_WINDOW_SIZE() / blockSize / chunkSize);
}

TileType Map::GetTileUnderCursor(const Vec2 &cameraPosition, const tiles_t &tiles) const
{
	Vec2 mousePos = mw::Window::GetMousePosition() - mw::Window::GetSize() / 2.0f;
	mousePos.y = mw::Window::GetSize().y - mw::Window::GetMousePosition().y - mw::Window::GetSize().y / 2.0f;
	Vec2 mousePosWorldSpace = cameraPosition + mousePos;
	Vec2 tilePos = mousePosWorldSpace / Vec2(GetBlockSize());

	tilePos = round(tilePos);

	return tiles[tilePos.x][tilePos.y].type;
}

BlockType Map::GetBlockUnderCursor(const Vec2 &cameraPosition) const
{
	return GetTileUnderCursor(cameraPosition, blocks);
}

WallType Map::GetWallUnderCursor(const Vec2 &cameraPosition) const
{
	return GetTileUnderCursor(cameraPosition, walls);
}

Map::BlockSettingData Map::Place(const Vec2 &cameraPosition, BlockType blockType, TilePos tilePos)
{
	Vec2 mousePos = mw::Window::GetMousePosition() - mw::Window::GetSize() / 2.0f;
	mousePos.y = mw::Window::GetSize().y - mw::Window::GetMousePosition().y - mw::Window::GetSize().y / 2.0f;
	Vec2 mousePosWorldSpace = cameraPosition + mousePos;

	Vec2 chunkPos = mousePosWorldSpace / Vec2(GetBlockSize() * GetChunkSize());
	Vec2 blockPos = mousePosWorldSpace / Vec2(GetBlockSize());

	blockPos = round(blockPos);

	auto &block = (tilePos == TilePos::Foreground ? blocks : walls)[blockPos.x][blockPos.y];

	BlockSettingData result;
	if ((blockType == BlockType::Empty && blockType != block.type) || (blockType != BlockType::Empty && block.type == BlockType::Empty))
	{
		result.blockType = block.type;
		result.oldBlock = block.type;

		block.type = blockType;

		result.block = blockPos;
		result.chunk = blockPos / GetChunkSize();
	}

	if (result.IsSet())
	{
		blocksUpdated = true;
		chunksUpdated = true;

		if (tilePos == TilePos::Foreground)
		{
			blockToUpdate = blockPos;
		}
		else
		{
			wallToUpdate = blockPos;
		}
	}

	return result;
}

Map::BlockSettingData Map::PlaceBlock(const Vec2 &cameraPosition, BlockType blockType)
{
	return Place(cameraPosition, blockType, TilePos::Foreground);
}

Map::BlockSettingData Map::PlaceWall(const Vec2 &cameraPosition, WallType wallType)
{
	return Place(cameraPosition, wallType, TilePos::Background);
}

void Map::CalculateVisibleChunks(Vec2 viewPos)
{
	visibleChunks.x.start = (viewPos.x - MW_WINDOW_WIDTH() / 2.0f) / 16.0f / GetChunkSize().x - 2;
	visibleChunks.x.end = visibleChunks.x.start + 15 + 4;

	visibleChunks.y.start = (viewPos.y - MW_WINDOW_HEIGHT() / 2.0f) / 16.0f / GetChunkSize().y - 2;
	visibleChunks.y.end = visibleChunks.y.start + 9 + 4;

	// const Vec2 middle = WhatChunk(GetCenter());
	// const Vec2 centeredViewPos = viewPos - (GetCenter() - GetChunkSize() * 2.0f) * blockSize;
	// const Vec2 chunkSizeInPixels = GetChunkSize() * blockSize;
	// const Vec2 shift = (mw::Window::GetSize() / chunkSizeInPixels / 2.0f);
	// const Vec2 additionalBlocks = Vec2(-1.0f); // when map size is 25x25 chunks.

	// visibleChunks.x.start = middle.x - shift.x + ceilf(centeredViewPos.x / chunkSizeInPixels.x) + additionalBlocks.x * 3;
	// visibleChunks.x.end = middle.x + shift.x + ceilf(centeredViewPos.x / chunkSizeInPixels.x) + additionalBlocks.x;
	// visibleChunks.y.start = middle.y - shift.y + ceilf(centeredViewPos.y / chunkSizeInPixels.y) + additionalBlocks.y * 3;
	// visibleChunks.y.end = middle.y + shift.y + ceilf(centeredViewPos.y / chunkSizeInPixels.y) + additionalBlocks.y;

	// visibleChunks.x.start = std::max(visibleChunks.x.start, 0);
	// visibleChunks.y.start = std::max(visibleChunks.y.start, 0);

	// visibleChunks.x.end = std::min(visibleChunks.x.end, static_cast<int>(GetAmountOfChunks().x) - 1);
	// visibleChunks.y.end = std::min(visibleChunks.y.end, static_cast<int>(GetAmountOfChunks().y) - 1);
}

bool Map::CheckBounds(int x, int y) const
{
	return x >= 0 && x < blocks.size() && y >= 0 && y < blocks[0].size();
}

void Map::SetBlock(int x, int y, BlockType type)
{
	if (CheckBounds(x, y))
	{
		blocks[x][y].type = type;
	}
}

bool Map::HasNeighbor(int x, int y, BlockType block) const
{
	if (x < 1 || x > blocks.size() - 2)
		return false;

	return blocks[x - 1][y + 1].type == block || blocks[x - 1][y].type == block || blocks[x + 1][y].type == block || blocks[x][y + 1].type == block;
}

bool Map::HasEmptyNeighbor(int x, int y) const
{
	return HasNeighbor(x, y, BlockType::Empty);
}

void Map::GenerateMap(MapGenerationDataSet generationDataSet)
{
	srand(generationDataSet.seed);

	amountOfBlocks = GetChunkSize() * GetAmountOfChunks();
	const int amountOfColumns = amountOfBlocks.x;
	const int amountOfRows = amountOfBlocks.y;

	blocks.resize(amountOfColumns);
	for (int x = 0; x < amountOfColumns; x++)
	{
		blocks[x].resize(amountOfRows);
	}

	walls = blocks;
}

Vec2 Map::WhatChunk(Vec2 block) const
{
	int x = static_cast<int>(truncf(block.x / GetChunkSize().x));
	int y = static_cast<int>(truncf(block.y / GetChunkSize().y));

	return {x, y};
}

chunk_t Map::WhatBlocks(Vec2 chunk) const
{
	Period<> x{static_cast<int>(chunk.x * GetChunkSize().x), static_cast<int>((chunk.x + 1.0f) * GetChunkSize().x)};
	Period<> y{static_cast<int>(chunk.y * GetChunkSize().y), static_cast<int>((chunk.y + 1.0f) * GetChunkSize().y)};
	return {x, y};
}

Vec2 Map::WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4 &projectionMatrix, const Mat4 &viewMatrix) const
{
	const Vec2 &viewPos = viewMatrix[0];

	const Vec2 screenCoords = (windowCoords / mw::Window::GetSize() - Vec2(0.5f, 0.5f)) * Vec2(1.0f, -1.0f) * 2.0f;
	const Vec4 projCoords = Math::Inverse(projectionMatrix) * Vec4(screenCoords, 0.0f, 1.0f);
	const Vec4 projViewCoords = Math::Inverse(viewMatrix) * projCoords;

	static const Vec2 normalization = Vec2(0.0f);
	const Vec2 block = (Vec2(projViewCoords) - viewPos) / blockSize + normalization + GetChunkSize() / 2.0f;

	return block;
}

Vec2 Map::GetChunkSize() const
{
	return chunkSize;
}

Vec2 Map::GetAmountOfChunks() const
{
	return amountOfChunks;
}

Vec2 Map::GetCenter() const
{
	return amountOfBlocks / 2.0f;
}

bounds_t &Map::GetVisibleChunks()
{
	return visibleChunks;
}

bounds_t &Map::GetLastVisibleChunks()
{
	return lastVisibleChunks;
}

float Map::GetBlockSize() const
{
	return blockSize;
}

blocks_t &Map::GetBlocks()
{
	return blocks;
}

walls_t &Map::GetWalls()
{
	return walls;
}

bool Map::BlockIs(int x, int y, BlockType type) const
{
	return blocks[x][y].type == type;
}

bool Map::WallIs(int x, int y, WallType type) const
{
	return walls[x][y].type == type;
}

bool Map::BlockIsEmpty(int x, int y) const
{
	return BlockIs(x, y, BlockType::Empty);
}

bool Map::WallIsEmpty(int x, int y) const
{
	return WallIs(x, y, WallType::Empty);
}

int Map::GetWidth() const
{
	return blocks.size();
}

int Map::GetHeight() const
{
	return blocks[0].size();
}

int Map::GetArea() const
{
	return GetWidth() * GetHeight();
}

int Map::GetSizeInBytes() const
{
	return GetArea() * sizeof(Tile) * 2 /* blocks AND walls */;
}

int Map::GetSizeInKilobytes() const
{
	return BytesTo(GetSizeInBytes(), SizeUnits::Kilobyte);
}

int Map::GetSizeInMegabytes() const
{
	return BytesTo(GetSizeInBytes(), SizeUnits::Megabyte);
}