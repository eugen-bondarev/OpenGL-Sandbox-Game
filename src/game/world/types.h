#pragma once

#include "blocks.h"
#include "util/structures.h"

struct Tile
{
	TileType type{TileType::Empty};
	Vec2 texture{-1};
};

enum class TilePos
{
	Background,
	Foreground
};

using tiles_row_t = std::vector<Tile>;
using tiles_t = std::vector<tiles_row_t>;

using blocks_t = tiles_t;
using walls_t = tiles_t;

using bounds_t = struct __bounds_t
{
	Period<> x;
	Period<> y;

	inline bool operator!=(__bounds_t const &other) const
	{
		return x.start != other.x.start || x.end != other.x.end || y.start != other.y.start || y.end != other.y.end;
	}

	inline bool operator<(__bounds_t const &other) const
	{
		return x < other.x || (x == other.x && y < other.y);
	}

	inline int GetArea() const
	{
		return (x.end - x.start) * (y.end - y.start);
	}
};

using chunk_t = bounds_t;

using light_data_t = std::vector<Vec2>;