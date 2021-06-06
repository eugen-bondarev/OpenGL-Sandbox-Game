#pragma once

#include "Core/Window.h"

#include "Util/Structures.h"

#include "Game/Blocks.h"
#include "Game/Types.h"
#include "Game/Chunk.h"
#include "Game/Map.h"

#include "Resources/ChunkFBO.h"

#include "GPU/VAO.h"
#include "GPU/Texture.h"
#include "GPU/Shader.h"

#include "Math/Math.h"

class MapRenderer {
public:
	MapRenderer(Ref<Map>& map);

	inline Size GetChunkSizePixels() const {
		return map->GetChunkSize() * map->GetBlockSize();
	}

  template <typename... Args>
  inline Chunk& GetChunk(Args... args) {
		Vec2 index = { args... };
    return chunks[index.x][index.y];
  }

	template <typename... Args>
	inline void RerenderChunk(Args... args) {
		auto& chunk = GetChunk(args...);
		chunk.Rerender();
	}

	void UpdateNeighborChunks(const Pos& chunk, const Pos& block);

	void Prerender();

	using chunks_row_t = std::vector<Chunk>;
	using chunks_t = std::vector<chunks_row_t>;
	chunks_t chunks;

private:
	Ref<Map> map;

	void InitGraphics();

	Ref<VAO> tileVao;
	Ref<VBO> dynVBO;
	Ref<Texture> tileMapTexture;
	Ref<Shader> shader;
};