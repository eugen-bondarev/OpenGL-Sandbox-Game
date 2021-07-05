#pragma once

#include "maths/maths.h"

enum class TileType
{
	Empty = 0,
	Dirt,
	Grass,
	Stone,
	Wood,
};

namespace Blocks {
namespace Representations {

struct IndexAndPosition
{
	Vec2 index;
	Vec2 worldPosition;
};

}
}

using BlockType = TileType;
using WallType = TileType;