#include "Map.h"

Map::Map(Size size) {
    this->size = size;

    GenerateMap(blocks, size);
}

void GenerateMap(Map::blocks_t& map, Size size) {
    const int HORIZONT_LINE = 500;

    int width = static_cast<int>(size.width);
    int height = static_cast<int>(size.height);

    map.resize(width);
    for (int i = 0; i < width; i++) {
        map[i].resize(height);
        for (int j = 0; j < height; j++) {
            map[i][j] = BlockType::Empty;
        }
    }

    for (int x = 0; x < width; x++) {
        map[x][HORIZONT_LINE] = BlockType::Grass;
    }

    for (int y = 0; y < HORIZONT_LINE; y++) {
        for (int x = 0; x < width; x++) {
            map[x][y] = BlockType::Dirt;
        }
    }
}