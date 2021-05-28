#pragma once

#include "Core/Window.h"

#include "Blocks.h"

#include "Util/Structures.h"

#include "Chunk.h"

#include "Gpu/ChunkFbo.h"
#include "Gpu/Vao.h"
#include "Gpu/Texture.h"
#include "Gpu/Shader.h"

#include "Math/Math.h"

#include "Types.h"

class Map {
public:
	Map(Size chunkSize, Size amountOfChunks);

	using chunks_row_t = std::vector<Chunk>;
	using chunks_t = std::vector<chunks_row_t>;

	inline Pos WhatChunk(Pos block) const {
		int x = static_cast<int>(truncf(block.x / chunkSize.x));
		int y = static_cast<int>(truncf(block.y / chunkSize.y));

		return { x, y };
	}

	inline chunk_t WhatBlocks(Pos chunk) const {
		Period<> x{ static_cast<int>(chunk.x * chunkSize.x), static_cast<int>((chunk.x + 1.0f) * chunkSize.x) };
		Period<> y{ static_cast<int>(chunk.y * chunkSize.y), static_cast<int>((chunk.y + 1.0f) * chunkSize.y) };
		return { x, y };
	}

	blocks_t blocks;
	chunks_t chunks;

	inline Pos GetCenter() const {
		return amountOfBlocks / 2.0f;
	}

	inline Size GetChunkSize() const {
		return chunkSize;
	}

	inline Size GetChunkSizePixels() const {
		return chunkSize * BLOCK_SIZE;
	}

	inline Size GetAmountOfChunks() const {
		return amountOfChunks;
	}

	inline Vec2 WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4& projectionMatrix, const Mat4& viewMatrix) const {
		const Vec2& viewPos = viewMatrix[0];

		const Vec2 screenCoords = (windowCoords / Window::GetSize() - Vec2(0.5f, 0.5f)) * Vec2(1.0f, -1.0f) * 2.0f;
		const Vec4 projCoords = Math::Inverse(projectionMatrix) * Vec4(screenCoords, 0.0f, 1.0f);
		const Vec4 projViewCoords = Math::Inverse(viewMatrix) * projCoords;

		const Vec2 cameraPosInMap = Vec2(viewPos / static_cast<float>(BLOCK_SIZE)) + chunkSize * amountOfChunks / 2.0f;

		static const Vec2 normalization = Vec2(0.0f);
		const Vec2 block = (Vec2(projViewCoords) - viewPos) / static_cast<float>(BLOCK_SIZE) + normalization + chunkSize / 2.0f;
		
		return block;
	}

	void InitGraphics();
	std::shared_ptr<Vao> tileVao;
	std::shared_ptr<Texture> tileMap;
	std::shared_ptr<Shader> shader;

	Mat4 viewMatrix{Mat4(1)};
	Vec2 viewPos{Vec2(0)};

	Mat4 projMatrix{Mat4(1)};

private:
	Size chunkSize;
	Size amountOfChunks;
	Size amountOfBlocks;

	void GenerateMap();
};