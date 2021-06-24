#pragma once

enum class BlockType {
	Empty = 0,
	Dirt,
	Grass
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