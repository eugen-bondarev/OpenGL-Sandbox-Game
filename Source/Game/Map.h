#pragma once

#include "BlockType.h"

#include <vector>

class Map {
public:
    Map(Size size);

    using block_row_t = std::vector<BlockType>;
    using blocks_t = std::vector<block_row_t>;

    inline Size GetSize() const {
        return size;
    }

    blocks_t blocks;
private:

    Size size;
};

void GenerateMap(Map::blocks_t& map, Size size);