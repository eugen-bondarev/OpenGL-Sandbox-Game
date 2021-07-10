#include "map.h"

#include "FastNoise/FastNoiseLite.h"

#include "renderer/world/map/tiles.h"

#include "renderer/atlas/texture_atlas.h"

#include "renderer/world/map/tiles.h"

FastNoiseLite noise1;
std::map<Vec2, ChunkData, Compare> chunkData;

std::vector<Vec4> renderData;

float WhatNoise(float x, float y)
{
	static float size = 0.4f;
	return noise1.GetNoise(x * size, y * size) * 0.5f + 0.5f;
}

BlockType WhatBlockType(float noiseValue, TilePos tilePos, float x, float y)
{
	BlockType type = BlockType::Empty;

	if (y > 4075 * 5 + 500 * noise1.GetNoise(x * 0.1f, 0.0f))
	{
		return BlockType::Empty;
	}
	
	if (tilePos == TilePos::Foreground)
	{
		if (noiseValue > 0.75f) 
		{
			type = BlockType::Empty;
		}
		if (noiseValue >= 0.3f && noiseValue < 0.75f)
		{
			type = BlockType::Grass;
		}
		if (noiseValue < 0.3f)
		{
			type = BlockType::Stone;
		}
	}
	else
	{
		if (noiseValue > 0.75f) 
		{
			type = BlockType::Grass;
		}
		if (noiseValue >= 0.3f && noiseValue < 0.75f)
		{
			type = BlockType::Grass;
		}
		if (noiseValue < 0.3f)
		{
			type = BlockType::Stone;
		}
	}

	return type;
}

BlockRepresentation WhatBlock(float noiseValue, TilePos tilePos, float x, float y)
{
	BlockRepresentation representation;

	representation.type = WhatBlockType(noiseValue, tilePos, x, y);
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

void ConvertChunksRenderData(Map* map, std::vector<Vec4>& data)
{
	BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);
	
	// noise1.SetNoiseType(FastNoiseLite::NoiseType_Value); REALLY LIKE IT!
	noise1.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

	auto& visibleChunks = map->GetVisibleChunks();

	for (int xChunk = visibleChunks.x.start; xChunk < visibleChunks.x.end; xChunk++)
	{
		for (int yChunk = visibleChunks.y.start; yChunk < visibleChunks.y.end; yChunk++)
		{
			Vec2 chunkIndices = Vec2(xChunk, yChunk);

			for (int x = 0; x < map->GetChunkSize().x; x++)
			{
				for (int y = 0; y < map->GetChunkSize().y; y++)
				{
					Vec2 blockPosition = (chunkIndices * map->GetChunkSize() + Vec2(x, y)) * 16.0f;
					
					float noiseValue = WhatNoise(blockPosition.x, blockPosition.y);
					BlockRepresentation computedBlock = WhatBlock(noiseValue, TilePos::Foreground, blockPosition.x, blockPosition.y);
					BlockRepresentation computedWall = WhatBlock(noiseValue, TilePos::Background, blockPosition.x, blockPosition.y);

					int xi = static_cast<int>(truncf((xChunk - visibleChunks.x.start) * map->GetChunkSize().x + x));
					int yi = static_cast<int>(truncf((yChunk - visibleChunks.y.start) * map->GetChunkSize().y + y));

					map->BLOCKS[xi][yi].type = computedBlock.type;
					map->BLOCKS[xi][yi].worldPosition = computedBlock.position;

					map->WALLS[xi][yi].type = computedWall.type;
					map->WALLS[xi][yi].worldPosition = computedWall.position;
				}
			}
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
	Vec2 correction = Vec2(-16, 54);

	visibleChunks.x.start = (viewPos.x + correction.x - MW_WINDOW_WIDTH() / 2.0f) / 16.0f / GetChunkSize().x - 0;
	visibleChunks.x.end = visibleChunks.x.start + MW_WINDOW_WIDTH() / (GetChunkSize().x * blockSize) + 2;

	visibleChunks.y.start = (viewPos.y + correction.y - MW_WINDOW_HEIGHT() / 2.0f) / 16.0f / GetChunkSize().y - 1;
	visibleChunks.y.end = visibleChunks.y.start + MW_WINDOW_HEIGHT() / (GetChunkSize().y * blockSize) + 2;
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







blocks_t &Map::GetBlocks1()
{
	return BLOCKS;
}

walls_t &Map::GetWalls1()
{
	return WALLS;
}

bool Map::BlockIs1(int x, int y, BlockType type) const
{
	// if (x < 0 || y < 0 || x >= BLOCKS.size() || y >= BLOCKS[0].size()) return false;

	return BLOCKS[x][y].type == type;
}

bool Map::WallIs1(int x, int y, WallType type) const
{
	// if (x < 0 || y < 0 || x >= WALLS.size() || y >= WALLS[0].size()) return false;

	return WALLS[x][y].type == type;
}

bool Map::BlockIsEmpty1(int x, int y) const
{
	return BlockIs1(x, y, BlockType::Empty);
}

bool Map::WallIsEmpty1(int x, int y) const
{
	return WallIs1(x, y, WallType::Empty);
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