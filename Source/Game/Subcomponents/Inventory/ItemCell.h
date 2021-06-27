#pragma once

#include "ItemType.h"

#include "Game/Blocks.h"

struct ItemCell {
  ItemType type { ItemType::Empty };
  int quantity  { 0 };

  struct {
    BlockType blockType;
    WallType wallType;
  } data;

  bool IsEmpty() const {
    return type == ItemType::Empty;
  }
};