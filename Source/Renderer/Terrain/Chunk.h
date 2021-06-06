#pragma once

#include "Game/Types.h"

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
    bounds_t bounds,
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

  inline const light_data_t& GetLightData() const {
    return lightData;
  }

  bool containsOnlyEmptyBlocks { false };

  void Rerender(
    Ref<Shader>& shader, 
    Ref<VAO>& vao, 
    Ref<VBO>& dynVBO, 
    Ref<Texture>& tileMap,
    const blocks_t& blocks
  );

  void Render(Ref<Shader>& shader);

private:
  Ref<Texture> targetTexture;

  light_data_t lightData;

  Pos chunkPos;
  Size chunkSize;
  bounds_t bounds;
  float blockSize;
};