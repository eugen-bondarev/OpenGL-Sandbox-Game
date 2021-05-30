#pragma once

#include "Core/Window.h"

#include "Blocks.h"
#include "Types.h"

class Map {
public:
  Map(Size chunkSize, Size amountOfChunks);

	inline Pos WhatChunk(Pos block) const {
		int x = static_cast<int>(truncf(block.x / GetChunkSize().x));
		int y = static_cast<int>(truncf(block.y / GetChunkSize().y));

		return { x, y };
	}

	inline chunk_t WhatBlocks(Pos chunk) const {
		Period<> x{ static_cast<int>(chunk.x * GetChunkSize().x), static_cast<int>((chunk.x + 1.0f) * GetChunkSize().x) };
		Period<> y{ static_cast<int>(chunk.y * GetChunkSize().y), static_cast<int>((chunk.y + 1.0f) * GetChunkSize().y) };
		return { x, y };
	}

	inline Vec2 WindowCoordsToBlockCoords(Vec2 windowCoords, const Mat4& projectionMatrix, const Mat4& viewMatrix) const {
		const Vec2& viewPos = viewMatrix[0];

		const Vec2 screenCoords = (windowCoords / Window::GetSize() - Vec2(0.5f, 0.5f)) * Vec2(1.0f, -1.0f) * 2.0f;
		const Vec4 projCoords = Math::Inverse(projectionMatrix) * Vec4(screenCoords, 0.0f, 1.0f);
		const Vec4 projViewCoords = Math::Inverse(viewMatrix) * projCoords;

		const Vec2 cameraPosInMap = Vec2(viewPos / static_cast<float>(BLOCK_SIZE)) + GetChunkSize() * GetAmountOfChunks() / 2.0f;

		static const Vec2 normalization = Vec2(0.0f);
		const Vec2 block = (Vec2(projViewCoords) - viewPos) / static_cast<float>(BLOCK_SIZE) + normalization + GetChunkSize() / 2.0f;
		
		return block;
	}

  inline Size GetChunkSize() const {
    return chunkSize;
  }

  inline Size GetAmountOfChunks() const {
    return amountOfChunks;
  }

	inline Pos GetCenter() const {
		return amountOfBlocks / 2.0f;
	}

	bounds_t GetVisibleChunks(Pos viewPos);

	blocks_t blocks;

private:
  Size chunkSize;
  Size amountOfChunks;
	Size amountOfBlocks;

	void GenerateMap();
};