#include "Map.h"

Map::Map(Size chunkSize, Size amountOfChunks) {
  this->chunkSize = chunkSize;
  this->amountOfChunks = amountOfChunks;

	GenerateMap();
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