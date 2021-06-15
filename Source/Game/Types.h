#pragma once

#include "Blocks.h"
#include "Util/Structures.h"

#include <vector>

using blocks_row_t = std::vector<BlockType>;
using blocks_t = std::vector<blocks_row_t>;

using walls_t = blocks_t;

using bounds_t = struct __bounds_t {
  Period<> x;
  Period<> y;

  inline bool operator!=(__bounds_t const& other) const {
    return x.start != other.x.start || x.end != other.x.end || y.start != other.y.start || y.end != other.y.end;
  }

  inline bool operator<(__bounds_t const& other) const {
    return x < other.x || (x == other.x && y < other.y);
  }

  inline int GetArea() const {
    return (x.end - x.start) * (y.end - y.start);
  }
};

using chunk_t = bounds_t;

using light_data_t = std::vector<Pos>;