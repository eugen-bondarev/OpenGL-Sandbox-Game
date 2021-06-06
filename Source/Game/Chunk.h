#pragma once

#include "Types.h"

#include "GPU/Texture.h"
#include "GPU/Shader.h"
#include "GPU/VAO.h"

inline static std::map<BlockType, Pos> tileMapDictionary = {
  { BlockType::Dirt, { 1, 1 } }
};

class Chunk {
public:
  Chunk(
    Pos chunkPos, 
    Size chunkSize,
    Ref<Shader> shader, 
    Ref<VAO> vao, 
    Ref<Texture> tileMap, 
    bounds_t bounds,
    blocks_t& blocks,
    float blockSize
  );

  inline Ref<Texture>& GetTargetTexture() {
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
  
  bool highlight { false };

  bool containsOnlyEmptyBlocks { false };

  void Rerender();
  void Render(Ref<Shader>& shader);

  light_data_t lightData;

private:
  Ref<Texture> targetTexture;

  blocks_t& blocks;

  float blockSize;
  
  Ref<Shader> shader;
  Ref<VAO> vao;
  Ref<Texture> tileMapTexture;

  Pos chunkPos;
  Size chunkSize;
  bounds_t bounds;
};