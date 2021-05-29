#pragma once

#include "Core/Window.h"

#include "Util/Structures.h"

#include "Game/Blocks.h"
#include "Game/Types.h"
#include "Game/Chunk.h"
#include "Game/Map.h"

#include "Gpu/ChunkFbo.h"
#include "Gpu/Vao.h"
#include "Gpu/Texture.h"
#include "Gpu/Shader.h"

#include "Math/Math.h"

class MapRenderer {
public:
	MapRenderer(std::shared_ptr<Map>& map);

	using chunks_row_t = std::vector<Chunk>;
	using chunks_t = std::vector<chunks_row_t>;

	chunks_t chunks;

	inline Size GetChunkSizePixels() const {
		return map->GetChunkSize() * BLOCK_SIZE;
	}

	void Prerender();

private:
	std::shared_ptr<Map> map;

	void InitGraphics();

	std::shared_ptr<Vao> tileVao;
	std::shared_ptr<Texture> tileMapTexture;
	std::shared_ptr<Shader> shader;
};