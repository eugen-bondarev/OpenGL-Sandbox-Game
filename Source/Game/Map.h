#pragma once

#include "Core/Window.h"

#include "BlockType.h"

#include "Gpu/Texture.h"
#include "Gpu/Vao.h"
#include "Gpu/Shader.h"

#include "LightData.h"

#include <vector>

class Map {
public:
    Map(Size size, int blockSizeInPixels);

    using block_row_t = std::vector<BlockView>;
    using blocks_t = std::vector<block_row_t>;
    using chunk_t = struct {
        Period<float> x;
        Period<float> y;
    };

    inline chunk_t GetChunk() const {
        return chunk;
    }

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

    inline Vec2 WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4& projectionMatrix, const Mat4& viewMatrix) const {
        const Vec2& viewPos = viewMatrix[0];

        const Vec2 screenCoords = (windowCoords / Window::GetSize() - Vec2(0.5f, 0.5f)) * Vec2(1.0f, -1.0f) * 2.0f;
        const Vec4 projCoords = Math::Inverse(projectionMatrix) * Vec4(screenCoords, 0.0f, 1.0f);
        const Vec4 projViewCoords = Math::Inverse(viewMatrix) * projCoords;

        const Vec2 cameraPosInMap = Vec2(viewPos / static_cast<float>(GetBlockSize())) + GetSize() / 2.0f;

        static const Vec2 normalization = Vec2(0.5f);
        const Vec2 block = (Vec2(projViewCoords) - viewPos) / static_cast<float>(GetBlockSize()) + cameraPosInMap + normalization;
        
        return block;
    }

    LightData Render(std::shared_ptr<Shader>& shader, Vec2 viewPos);

    blocks_t blocks;

    inline void RecalculateLight() {
        recalculateLight = true;
    }

private:
    std::shared_ptr<Texture> tileMap;
    std::shared_ptr<Vao> quadVao;

    LightData lightData;
    bool recalculateLight { true };

    Vec2 middleOfMap;

    chunk_t chunk;

    void CalculateEmptyNeighbours(int x, int y);
    Vec2 PickTile(int x, int y);

    inline static std::map<BlockType, Vec2> tileDictionary = {
        { BlockType::Grass, Vec2(0, 0) + Vec2(1, 1) },
        { BlockType::Dirt,  Vec2(3, 0) + Vec2(1, 1) },
        { BlockType::Stone, Vec2(6, 0) + Vec2(1, 1) },
    };

    Size size;
    int blockSizeInPixels;
};

void GenerateMap(Map::blocks_t& map, Size size);