#pragma once

#include "game/world/types.h"

bool TopBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);
bool LeftBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);
bool BottomBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);
bool RightBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type = BlockType::Empty);

using TileFunction = std::function<Vec2(const blocks_t &, int, int)>;

Vec2 GrassGenerator(const blocks_t &blocks, int x, int y);
Vec2 DirtGenerator(const blocks_t &blocks, int x, int y);

inline static std::map<BlockType, TileFunction> tileFunctions = {
	{ BlockType::Grass, GrassGenerator},
	{ BlockType::Dirt, DirtGenerator},
};

TileFunction PickTileFunction(TileType type);

Vec2 PickRightAngularBlock(const blocks_t &blocks, int x, int y);
Vec2 PickRightAngularWall(const blocks_t &blocks, int x, int y);