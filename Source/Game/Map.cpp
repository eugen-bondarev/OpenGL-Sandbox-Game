#include "Map.h"

Map::Map(Size chunkSize, Size amountOfChunks) {
  this->chunkSize = chunkSize;
  this->amountOfChunks = amountOfChunks;

	GenerateMap();
}

bounds_t Map::GetVisibleChunks(Pos viewPos) {
	const Pos middle = WhatChunk(GetCenter());
	const Vec2 centeredViewPos = viewPos - GetCenter() * BLOCK_SIZE;
	const Vec2 additionalBlocks = Vec2(2, 2);
	const Vec2 chunkSizeInPixels = GetChunkSize() * BLOCK_SIZE;
	const Vec2 shift = (Window::GetSize() / chunkSizeInPixels / 2.0f);
	
	bounds_t bounds;
	bounds.x.start = middle.x - shift.x + centeredViewPos.x / chunkSizeInPixels.x - additionalBlocks.x;
	bounds.x.end   = middle.x + shift.x + centeredViewPos.x / chunkSizeInPixels.x + additionalBlocks.x;
	bounds.y.start = middle.y - shift.y + centeredViewPos.y / chunkSizeInPixels.y - additionalBlocks.y;
	bounds.y.end   = middle.y + shift.y + centeredViewPos.y / chunkSizeInPixels.y + additionalBlocks.y;

	return bounds;
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
}