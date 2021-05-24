#pragma once

#include "BlockType.h"

#include "Gpu/Texture.h"
#include "Gpu/Vao.h"
#include "Gpu/Shader.h"

#include <vector>

class Map {
public:
    Map(Size size, int blockSizeInPixels);

    using block_row_t = std::vector<BlockType>;
    using blocks_t = std::vector<block_row_t>;
    using chunk_t = struct {
        Period<float> x;
        Period<float> y;
    };

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

    void Render(Shader& shader, Vec2 viewPos);

    blocks_t blocks;

private:
    std::shared_ptr<Texture> tileMap;
    std::shared_ptr<Vao> quadVao;

    Vec2 middleOfMap;

    inline static std::map<BlockType, Vec2> tileDictionary = {
        { BlockType::Grass, Vec2(1, 0) },
        { BlockType::Dirt, Vec2(4, 1) },
        { BlockType::Stone, Vec2(7, 0) },
    };

    Size size;
    int blockSizeInPixels;
};

void GenerateMap(Map::blocks_t& map, Size size);