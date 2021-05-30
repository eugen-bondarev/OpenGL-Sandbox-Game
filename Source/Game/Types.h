#pragma once

#include "Blocks.h"
#include "Util/Structures.h"

#include <vector>

inline static float BLOCK_SIZE { 16.0f };

using blocks_row_t = std::vector<BlockType>;
using blocks_t = std::vector<blocks_row_t>;

using bounds_t = struct {
  Period<> x;
  Period<> y;
};

using chunk_t = bounds_t;

using light_data_t = std::vector<Pos>;