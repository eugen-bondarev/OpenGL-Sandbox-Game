#include "Map.h"

Map::Map(Vec2 chunkSize, Vec2 amountOfChunks, float blockSize) : chunkSize { chunkSize }, amountOfChunks { amountOfChunks }, blockSize { blockSize } {
	GenerateMap();
}

Map::BlockSettingData Map::SetBlock(const Vec2& cameraPosition, BlockType blockType) {	
	Vec2 mousePos = Window::GetMousePosition() - Window::GetSize() / 2.0f;
	mousePos.y = Window::GetSize().y - Window::GetMousePosition().y - Window::GetSize().y / 2.0f;
	Vec2 mousePosWorldSpace = cameraPosition + mousePos;

	Vec2 chunkPos = mousePosWorldSpace / Vec2(GetBlockSize() * GetChunkSize());
	Vec2 blockPos = mousePosWorldSpace / Vec2(GetBlockSize());

	blockPos = round(blockPos);

	auto& block = blocks[blockPos.x][blockPos.y];

	BlockSettingData result;
	if ((blockType == BlockType::Empty && blockType != block) || (blockType != BlockType::Empty && block == BlockType::Empty)) {
		block = blockType;

		result.block = blockPos;
		result.chunk = blockPos / GetChunkSize();
	}

	return result;
}

void Map::CalculateVisibleChunks(Vec2 viewPos) {	
	const Vec2 middle = WhatChunk(GetCenter());
	const Vec2 centeredViewPos = viewPos - (GetCenter() - GetChunkSize() * 2.0f) * blockSize;
	const Vec2 chunkSizeInPixels = GetChunkSize() * blockSize;
	const Vec2 shift = (Window::GetSize() / chunkSizeInPixels / 2.0f);
	const Vec2 additionalBlocks = Vec2(-1.0f); // when map size is 25x25 chunks.
	
	visibleChunks.x.start = middle.x - shift.x + ceilf(centeredViewPos.x / chunkSizeInPixels.x) + additionalBlocks.x * 3;
	visibleChunks.x.end   = middle.x + shift.x + ceilf(centeredViewPos.x / chunkSizeInPixels.x) + additionalBlocks.x;
	visibleChunks.y.start = middle.y - shift.y + ceilf(centeredViewPos.y / chunkSizeInPixels.y) + additionalBlocks.y * 3;
	visibleChunks.y.end   = middle.y + shift.y + ceilf(centeredViewPos.y / chunkSizeInPixels.y) + additionalBlocks.y;

	visibleChunks.x.start = std::max(visibleChunks.x.start, 0);
	visibleChunks.y.start = std::max(visibleChunks.y.start, 0);

	visibleChunks.x.end = std::min(visibleChunks.x.end, static_cast<int>(GetAmountOfChunks().x) - 1);
	visibleChunks.y.end = std::min(visibleChunks.y.end, static_cast<int>(GetAmountOfChunks().y) - 1);
}

void Map::GenerateMap() {
	amountOfBlocks = GetChunkSize() * GetAmountOfChunks();
	const int amountOfColumns = amountOfBlocks.x;
	const int amountOfRows = amountOfBlocks.y;

	blocks.resize(amountOfColumns);
	for (int x = 0; x < amountOfColumns; x++) {
		blocks[x].resize(amountOfRows);
	}

	const int middle = static_cast<int>(amountOfBlocks.y / 2.0f);

	for (int x = 0; x < amountOfBlocks.x; x++) {
		for (int y = 0; y < middle; y++) {
			blocks[x][y] = BlockType::Dirt;
		}

		blocks[x][middle] = BlockType::Grass;

		for (int y = middle + 1; y < amountOfBlocks.y; y++) {
			blocks[x][y] = BlockType::Empty;
		}
	}

	walls = blocks;
}

Vec2 Map::WhatChunk(Vec2 block) const {
	int x = static_cast<int>(truncf(block.x / GetChunkSize().x));
	int y = static_cast<int>(truncf(block.y / GetChunkSize().y));

	return { x, y };
}

chunk_t Map::WhatBlocks(Vec2 chunk) const {
	Period<> x { static_cast<int>(chunk.x * GetChunkSize().x), static_cast<int>((chunk.x + 1.0f) * GetChunkSize().x) };
	Period<> y { static_cast<int>(chunk.y * GetChunkSize().y), static_cast<int>((chunk.y + 1.0f) * GetChunkSize().y) };
	return { x, y };
}

Vec2 Map::WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4& projectionMatrix, const Mat4& viewMatrix) const {
	const Vec2& viewPos = viewMatrix[0];

	const Vec2 screenCoords = (windowCoords / Window::GetSize() - Vec2(0.5f, 0.5f)) * Vec2(1.0f, -1.0f) * 2.0f;
	const Vec4 projCoords = Math::Inverse(projectionMatrix) * Vec4(screenCoords, 0.0f, 1.0f);
	const Vec4 projViewCoords = Math::Inverse(viewMatrix) * projCoords;

	static const Vec2 normalization = Vec2(0.0f);
	const Vec2 block = (Vec2(projViewCoords) - viewPos) / blockSize + normalization + GetChunkSize() / 2.0f;
	
	return block;
}

Vec2 Map::GetChunkSize() const {
	return chunkSize;
}

Vec2 Map::GetAmountOfChunks() const {
	return amountOfChunks;
}

Vec2 Map::GetCenter() const {
	return amountOfBlocks / 2.0f;
}

const bounds_t& Map::GetVisibleChunks() const {
	return visibleChunks;
}

float Map::GetBlockSize() const {
	return blockSize;
}

blocks_t& Map::GetBlocks() {
	return blocks;
}

const walls_t& Map::GetWalls() const {
	return walls;
}

bool Map::BlockIs(int x, int y, BlockType type) const {
	return blocks[x][y] == type;
}

bool Map::WallIs(int x, int y, WallType type) const {
	return walls[x][y] == type;
}

bool Map::BlockIsEmpty(int x, int y) const {
	return BlockIs(x, y, BlockType::Empty);
}

bool Map::WallIsEmpty(int x, int y) const {
	return WallIs(x, y, WallType::Empty);
}