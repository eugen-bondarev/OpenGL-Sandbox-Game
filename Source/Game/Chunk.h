#pragma once

#include "Gpu/Texture.h"

#include <memory>

#include "Types.h"

#include "Gpu/ChunkFbo.h"
#include "Gpu/Shader.h"
#include "Gpu/Vao.h"

class Chunk {
public:
  Chunk(Pos chunkPos, Size chunkSize, ChunkFbo** fbo, Shader* shader, Vao* vao, Texture* tileMap, chunk_t chunk, blocks_t* blocks);

  inline std::shared_ptr<Texture>& GetTexture() {
    return texture;
  }

  chunk_t chunk;

  blocks_t* blocks;

  ChunkFbo** fbo;
  Shader* shader;
  Vao* vao;
  Texture* tileMap;

	Vec2 viewPos;
	Mat4 viewMatrix;

  void Prepare();
  void Rerender();

private:
  std::shared_ptr<Texture> texture;

  Pos chunkPos;
  Size chunkSize;
};