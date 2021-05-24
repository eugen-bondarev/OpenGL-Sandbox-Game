#pragma once

#include "BlockType.h"

#include "Gpu/Texture.h"
#include "Gpu/Vao.h"

#include <vector>

class Map {
public:
    Map(Size size, int blockSizeInPixels);

    using block_row_t = std::vector<BlockType>;
    using blocks_t = std::vector<block_row_t>;

    inline Size GetSize() const {
        return size;
    }

    inline const std::shared_ptr<Texture> & GetTileMap() const {
        return tileMap;
    }

    inline const std::shared_ptr<Vao> & GetVao() const {
        return quadVao;
    }

    inline int GetBlockSize() const {
        return blockSizeInPixels;
    }

    blocks_t blocks;

private:
    std::shared_ptr<Texture> tileMap;
    std::shared_ptr<Vao> quadVao;

    Size size;
    int blockSizeInPixels;
};

void GenerateMap(Map::blocks_t& map, Size size);