#include "Map.h"

Map::Map(Size chunkSize, Size amountOfChunks) {
	this->chunkSize = chunkSize;
	this->amountOfChunks = amountOfChunks;
	amountOfBlocks = chunkSize * amountOfChunks;
	GenerateMap();
}

void Map::GenerateMap() {
	const int amountOfColumns = amountOfBlocks.x;
	const int amountOfRows = amountOfBlocks.y;

	blocks.resize(amountOfColumns);
	for (int x = 0; x < amountOfColumns; x++) {
		blocks[x].resize(amountOfRows);
	}

	int middle = static_cast<int>(amountOfBlocks.x / 2.0f);

	for (int x = 0; x < amountOfBlocks.x; x++) {
		for (int y = 0; y < middle; y++) {
			blocks[x][y] = BlockType::Empty;
		}

		for (int y = middle; y < amountOfBlocks.y; y++) {
			blocks[x][y] = BlockType::Dirt;
		}
	}
}