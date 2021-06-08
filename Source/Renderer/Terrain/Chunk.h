#pragma once

#include "Game/Types.h"

#include "GPU/Texture.h"
#include "GPU/Shader.h"
#include "GPU/VAO.h"

inline static std::map<BlockType, Pos> tileMapDictionary = {
  { BlockType::Dirt, { 1, 1 } }
};

inline static std::map<BlockType, Pos> wallsTileMapDictionary = {
  { BlockType::Dirt, { 4, 1 } }
};

class Chunk {
public:
  Chunk(
    Pos chunkPos, 
    Size chunkSize,    
    bounds_t bounds,
    float blockSize
  );

  void AddWallToRenderingData(std::vector<Vec4>& wallData, WallType wall, const walls_t& walls, int x, int y);
  void AddBlockToRenderingData(std::vector<Vec4>& blockData, BlockType block, const blocks_t& blocks, int x, int y);

  inline Ref<Werwel::Texture>& GetTargetTexture() {
    return targetTexture;
  }

  inline Pos GetChunkPos() const {
    return chunkPos;
  }

  inline Size GetChunkSize() const {
    return chunkSize;
  }

  inline const bounds_t& GetBoudns() const {
    return bounds;
  }

  inline const light_data_t& GetLightData() const {
    return lightData;
  }

  bool containsOnlyEmptyBlocks { false };

  void Rerender(
    Ref<Werwel::Shader>& shader, 
    Ref<Werwel::VAO>& vao, 
    Ref<Werwel::VBO>& dynVBO, 
    Ref<Werwel::Texture>& tileMap,
    const blocks_t& blocks,
    const walls_t& walls
  );

  void Render(Ref<Werwel::Shader>& shader);

private:
  Ref<Werwel::Texture> targetTexture;

  light_data_t lightData;

  Pos chunkPos;
  Size chunkSize;
  bounds_t bounds;
  float blockSize;

  Mat4 chunkModelMatrix;
};