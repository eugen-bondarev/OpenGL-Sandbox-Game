#pragma once

#include "Blocks.h"
#include "Util/Structures.h"

#include <vector>

using blocks_row_t = std::vector<BlockType>;
using blocks_t = std::vector<blocks_row_t>;

using bounds_t = struct __bounds_t {
  Period<> x;
  Period<> y;

  inline bool operator!=(__bounds_t const& other) const {
    return x.start != other.x.start || x.end != other.x.end || y.start != other.y.start || y.end != other.y.end;
  }
};

using chunk_t = bounds_t;

using light_data_t = std::vector<Pos>;