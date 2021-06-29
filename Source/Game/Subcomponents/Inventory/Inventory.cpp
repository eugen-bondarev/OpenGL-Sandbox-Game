#include "Inventory.h"

#include "../../Items/Empty.h"

Inventory::Inventory() {
  for (int i = 0; i < 10; i++)
    items.emplace_back(new Empty());

  GiveItem<Pickaxe>(ToolType::BronzePickaxe, 1);
  GiveItem<Shovel>(ToolType::BronzeShovel, 1);
  GiveItem<Axe>(ToolType::BronzeAxe, 1);

  GiveItem<Block>(BlockType::Wood, 256);
  GiveItem<Wall>(WallType::Wood, 256);
}