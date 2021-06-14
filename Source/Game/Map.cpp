#include "Map.h"

Map::Map(Size chunkSize, Size amountOfChunks, float blockSize) {
  this->chunkSize = chunkSize;
  this->amountOfChunks = amountOfChunks;
	this->blockSize = blockSize;

	chunks.resize(amountOfChunks.x);
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i].resize(amountOfChunks.y);
		
		for (int j = 0; j < chunks[i].size(); j++) {
			chunks[i][j].index = Pos(i, j);
		}
	}

	GenerateMap();
}

BlockSettingData Map::SetBlock(const Vec2& cameraPosition, BlockType blockType) {	
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

void Map::CalculateVisibleChunks(Pos viewPos) {	
	const Pos middle = WhatChunk(GetCenter());
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

		for (int y = middle; y < amountOfBlocks.y; y++) {
			blocks[x][y] = BlockType::Empty;
		}
	}

	walls = blocks;
}