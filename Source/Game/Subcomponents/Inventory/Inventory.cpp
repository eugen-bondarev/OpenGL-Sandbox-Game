#include "Inventory.h"

Inventory::Inventory() {
  cells.resize(10);

  cells[0].quantity = 500;
  cells[0].type = ItemType::Block;
  cells[0].data.blockType = BlockType::Wood;

  cells[1].quantity = 500;
  cells[1].type = ItemType::Wall;
  cells[1].data.wallType = WallType::Wood;
}