#include "Map.h"

Map::Map(Size chunkSize, Size amountOfChunks) {
  this->chunkSize = chunkSize;
  this->amountOfChunks = amountOfChunks;

	GenerateMap();
}

void Map::CalculateVisibleChunks(Pos viewPos) {
	const Pos middle = WhatChunk(GetCenter());
	const Vec2 centeredViewPos = viewPos - (GetCenter() - GetChunkSize() * 2.0f) * BLOCK_SIZE;
	const Vec2 chunkSizeInPixels = GetChunkSize() * BLOCK_SIZE;
	const Vec2 shift = (Window::GetSize() / chunkSizeInPixels / 2.0f);
	const Vec2 additionalBlocks = Vec2(1);
	
	visibleChunks.x.start = middle.x - shift.x + centeredViewPos.x / chunkSizeInPixels.x - additionalBlocks.x;
	visibleChunks.x.end   = middle.x + shift.x + centeredViewPos.x / chunkSizeInPixels.x;
	visibleChunks.y.start = middle.y - shift.y + centeredViewPos.y / chunkSizeInPixels.y - additionalBlocks.y;
	visibleChunks.y.end   = middle.y + shift.y + centeredViewPos.y / chunkSizeInPixels.y;

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
}