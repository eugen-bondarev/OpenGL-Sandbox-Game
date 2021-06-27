#pragma once

#include "Maths/Maths.h"

enum class BlockType {
	Empty = 0,
	Dirt,
	Grass,
	Stone,
	Wood,
};

namespace Blocks {
namespace Representations {

struct IndexAndPosition {
	Vec2 index;
	Vec2 worldPosition;
};

}
}

using WallType = BlockType;