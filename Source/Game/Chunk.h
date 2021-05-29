#pragma once

#include "Gpu/Texture.h"

#include <memory>

#include "Types.h"

#include "Gpu/ChunkFbo.h"
#include "Gpu/LightFbo.h"
#include "Gpu/Shader.h"
#include "Gpu/Vao.h"

inline static std::map<BlockType, Pos> tileMapDictionary = {
  { BlockType::Grass, { 0, 1 } },
  { BlockType::Dirt, { 0, 0 } }
};

class Chunk {
public:
  Chunk(
    Pos chunkPos, 
    Size chunkSize,
    std::shared_ptr<Shader> shader, 
    std::shared_ptr<Shader> lightShader, 
    std::shared_ptr<Vao> vao, 
    std::shared_ptr<Vao> lightVao, 
    std::shared_ptr<Texture> tileMap, 
    std::shared_ptr<Texture> lightTexture,
    bounds_t bounds, 
    blocks_t& blocks
  );

  inline std::shared_ptr<Texture>& GetTargetTexture() {
    return targetTexture;
  }

  inline std::shared_ptr<Texture>& GetTargetLightTexture() {
    return targetLightTexture;
  }

  void Rerender();
  void Render(std::shared_ptr<Shader>& shader);

private:
  std::shared_ptr<Texture> targetTexture;
  std::shared_ptr<Texture> targetLightTexture;

  blocks_t& blocks;
  
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Shader> lightShader;
  std::shared_ptr<Vao> vao;
  std::shared_ptr<Vao> lightVao;
  std::shared_ptr<Texture> tileMapTexture;
  std::shared_ptr<Texture> lightTexture;

  Pos chunkPos;
  Size chunkSize;
  bounds_t bounds;

  std::vector<Pos> light;
};