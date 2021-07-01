#pragma once

#include "Game/Types.h"

static bool TopBlockIsEmpty1(const blocks_t &blocks, int x, int y) {
  return y < blocks[x].size() - 1 && blocks[x][y + 1] == BlockType::Empty;
}

static bool LeftBlockIsEmpty1(const blocks_t &blocks, int x, int y) {
  return x > 0 && blocks[x - 1][y] == BlockType::Empty;
}

static bool BottomBlockIsEmpty1(const blocks_t &blocks, int x, int y) {
  return y > 0 && blocks[x][y - 1] == BlockType::Empty;
}

static bool RightBlockIsEmpty1(const blocks_t &blocks, int x, int y) {
  return x < blocks.size() - 1 && blocks[x + 1][y] == BlockType::Empty;
}

static bool TopBlockIsEmpty(const blocks_t &blocks, int x, int y) {
  return y < blocks[x].size() - 1 && blocks[x][y + 1] != blocks[x][y];
}

static bool LeftBlockIsEmpty(const blocks_t &blocks, int x, int y) {
  return x > 0 && blocks[x - 1][y] != blocks[x][y];
}

static bool BottomBlockIsEmpty(const blocks_t &blocks, int x, int y) {
  return y > 0 && blocks[x][y - 1] != blocks[x][y];
}

static bool RightBlockIsEmpty(const blocks_t &blocks, int x, int y) {
  return x < blocks.size() - 1 && blocks[x + 1][y] != blocks[x][y];
}

static Vec2 PickRightAngularTile(const blocks_t &blocks, int x, int y) {
  #define TOP() TopBlockIsEmpty(blocks, x, y)
  #define LEFT() LeftBlockIsEmpty(blocks, x, y)
  #define BOTTOM() BottomBlockIsEmpty(blocks, x, y)
  #define RIGHT() RightBlockIsEmpty(blocks, x, y)

  if (TOP() && LEFT() && BOTTOM() && RIGHT())
    return {1, 2};

  if (LEFT() && TOP() && RIGHT())
    return {-1, 3};

  if (LEFT() && BOTTOM() && RIGHT())
    return {-1, 4};

  if (TOP() && RIGHT() && BOTTOM())
    return {0, 3};

  if (TOP() && LEFT() && BOTTOM())
    return {0, 4};

  if (TOP() && LEFT())
    return {-1, -1};

  if (TOP() && RIGHT())
    return {1, -1};

  if (BOTTOM() && LEFT())
    return {-1, 1};

  if (BOTTOM() && RIGHT())
    return {1, 1};

  if (TOP() && BOTTOM())
    return {0, 2};

  if (LEFT() && RIGHT())
    return {-1, 2};

  if (TOP())
    return {0, -1};

  if (BOTTOM())
    return {0, 1};

  if (LEFT())
    return {-1, 0};

  if (RIGHT())
    return {1, 0};

  #undef TOP
  #undef BOTTOM
  #undef LEFT
  #undef RIGHT

  return Vec2(0, 0);
}

static Vec2 PickRightAngularTile1(const blocks_t &blocks, int x, int y) {
  #define TOP() TopBlockIsEmpty1(blocks, x, y)
  #define LEFT() LeftBlockIsEmpty1(blocks, x, y)
  #define BOTTOM() BottomBlockIsEmpty1(blocks, x, y)
  #define RIGHT() RightBlockIsEmpty1(blocks, x, y)

  if (TOP() && LEFT() && BOTTOM() && RIGHT())
    return {1, 2};

  if (LEFT() && TOP() && RIGHT())
    return {-1, 3};

  if (LEFT() && BOTTOM() && RIGHT())
    return {-1, 4};

  if (TOP() && RIGHT() && BOTTOM())
    return {0, 3};

  if (TOP() && LEFT() && BOTTOM())
    return {0, 4};

  if (TOP() && LEFT())
    return {-1, -1};

  if (TOP() && RIGHT())
    return {1, -1};

  if (BOTTOM() && LEFT())
    return {-1, 1};

  if (BOTTOM() && RIGHT())
    return {1, 1};

  if (TOP() && BOTTOM())
    return {0, 2};

  if (LEFT() && RIGHT())
    return {-1, 2};

  if (TOP())
    return {0, -1};

  if (BOTTOM())
    return {0, 1};

  if (LEFT())
    return {-1, 0};

  if (RIGHT())
    return {1, 0};

  #undef TOP
  #undef BOTTOM
  #undef LEFT
  #undef RIGHT

  return Vec2(0, 0);
}