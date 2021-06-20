#pragma once

enum class BlockType {
	Empty = 0,
	Dirt,
	Stone
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