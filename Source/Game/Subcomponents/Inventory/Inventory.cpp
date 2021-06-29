#include "Inventory.h"

#include "../../Items/Empty.h"

Inventory::Inventory() {
  for (int i = 0; i < 10; i++)
    items.emplace_back(new Empty());

  items[0].reset(new Block(BlockType::Wood, 25));
  items[1].reset(new Wall(WallType::Wood, 25));
}