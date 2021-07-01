#include "Tiles.h"

bool TopBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type) {
  return y < blocks[x].size() - 1 && blocks[x][y + 1].type != type;
}

bool LeftBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type) {
  return x > 0 && blocks[x - 1][y].type != type;
}

bool BottomBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type) {
  return y > 0 && blocks[x][y - 1].type != type;
}

bool RightBlockIsNot(const blocks_t &blocks, int x, int y, BlockType type) {
  return x < blocks.size() - 1 && blocks[x + 1][y].type != type;
}

#define CHECK_NEIGHBOURS(TOP, LEFT, BOTTOM, RIGHT) \
  if (TOP() && LEFT() && BOTTOM() && RIGHT())\
    return Vec2 {1, 2};\
  if (LEFT() && TOP() && RIGHT())\
    return Vec2 {-1, 3};\
  if (LEFT() && BOTTOM() && RIGHT())\
    return Vec2 {-1, 4};\
  if (TOP() && RIGHT() && BOTTOM())\
    return Vec2 {0, 3};\
  if (TOP() && LEFT() && BOTTOM())\
    return Vec2 {0, 4};\
  if (TOP() && LEFT())\
    return Vec2 {-1, -1};\
  if (TOP() && RIGHT())\
    return Vec2 {1, -1};\
  if (BOTTOM() && LEFT())\
    return Vec2 {-1, 1};\
  if (BOTTOM() && RIGHT())\
    return Vec2 {1, 1};\
  if (TOP() && BOTTOM())\
    return Vec2 {0, 2};\
  if (LEFT() && RIGHT())\
    return Vec2 {-1, 2};\
  if (TOP())\
    return Vec2 {0, -1};\
  if (BOTTOM())\
    return Vec2 {0, 1};\
  if (LEFT())\
    return Vec2 {-1, 0};\
  if (RIGHT())\
    return Vec2 {1, 0};\
  return Vec2 {0, 0}

Vec2 GrassGenerator(const blocks_t& blocks, int x, int y) {
  #undef TOP
  #undef LEFT
  #undef BOTTOM
  #undef RIGHT

  #define TOP()     TopBlockIsNot   (blocks, x, y, blocks[x][y].type) && TopBlockIsNot   (blocks, x, y, BlockType::Dirt)
  #define LEFT()    LeftBlockIsNot  (blocks, x, y, blocks[x][y].type) && LeftBlockIsNot  (blocks, x, y, BlockType::Dirt)
  #define BOTTOM()  BottomBlockIsNot(blocks, x, y, blocks[x][y].type) && BottomBlockIsNot(blocks, x, y, BlockType::Dirt)
  #define RIGHT()   RightBlockIsNot (blocks, x, y, blocks[x][y].type) && RightBlockIsNot (blocks, x, y, BlockType::Dirt)

  CHECK_NEIGHBOURS(TOP, LEFT, BOTTOM, RIGHT);
}

Vec2 DirtGenerator(const blocks_t& blocks, int x, int y) {
  #undef TOP
  #undef LEFT
  #undef BOTTOM
  #undef RIGHT

  #define TOP()     TopBlockIsNot   (blocks, x, y, blocks[x][y].type) && TopBlockIsNot   (blocks, x, y, BlockType::Grass)
  #define LEFT()    LeftBlockIsNot  (blocks, x, y, blocks[x][y].type) && LeftBlockIsNot  (blocks, x, y, BlockType::Grass)
  #define BOTTOM()  BottomBlockIsNot(blocks, x, y, blocks[x][y].type) && BottomBlockIsNot(blocks, x, y, BlockType::Grass)
  #define RIGHT()   RightBlockIsNot (blocks, x, y, blocks[x][y].type) && RightBlockIsNot (blocks, x, y, BlockType::Grass)

  CHECK_NEIGHBOURS(TOP, LEFT, BOTTOM, RIGHT);
}

Vec2 PickRightAngularBlock(const blocks_t &blocks, int x, int y) {
  #undef TOP
  #undef LEFT
  #undef BOTTOM
  #undef RIGHT

  #define TOP()     TopBlockIsNot   (blocks, x, y, blocks[x][y].type)
  #define LEFT()    LeftBlockIsNot  (blocks, x, y, blocks[x][y].type)
  #define BOTTOM()  BottomBlockIsNot(blocks, x, y, blocks[x][y].type)
  #define RIGHT()   RightBlockIsNot (blocks, x, y, blocks[x][y].type)
  
  CHECK_NEIGHBOURS(TOP, LEFT, BOTTOM, RIGHT);
}

Vec2 PickRightAngularWall(const blocks_t &blocks, int x, int y) {
  #undef TOP
  #undef LEFT
  #undef BOTTOM
  #undef RIGHT

  #define TOP()     !TopBlockIsNot   (blocks, x, y)
  #define LEFT()    !LeftBlockIsNot  (blocks, x, y)
  #define BOTTOM()  !BottomBlockIsNot(blocks, x, y)
  #define RIGHT()   !RightBlockIsNot (blocks, x, y)
  
  CHECK_NEIGHBOURS(TOP, LEFT, BOTTOM, RIGHT);
}

TileFunction PickTileFunction(TileType type) {
  bool special = tileFunctions.find(type) != tileFunctions.end();
  return special ? tileFunctions[type] : PickRightAngularBlock;
}