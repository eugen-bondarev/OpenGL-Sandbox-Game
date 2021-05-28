#pragma once

#include "Gpu/Texture.h"

#include <memory>

#include "Types.h"

#include "Gpu/ChunkFbo.h"
#include "Gpu/Shader.h"
#include "Gpu/Vao.h"

class Chunk {
public:
  Chunk(
    Pos chunkPos, 
    Size chunkSize,
    std::shared_ptr<Shader> shader, 
    std::shared_ptr<Vao> vao, 
    std::shared_ptr<Texture> tileMap, 
    chunk_t chunk, 
    blocks_t& blocks
  );

  inline std::shared_ptr<Texture>& GetTexture() {
    return texture;
  }

  void Rerender();

private:
  std::shared_ptr<Texture> texture;

  bounds_t bounds;

  blocks_t& blocks;

  ChunkFbo** fbo;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Vao> vao;
  std::shared_ptr<Texture> tileMap;

  Pos chunkPos;
  Size chunkSize;
};