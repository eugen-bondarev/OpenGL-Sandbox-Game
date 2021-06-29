#pragma once

#include "../../Items/Block.h"
#include "../../Items/Wall.h"

class Inventory {
friend class Player;
friend class InterfaceRenderer;

public:
  Inventory();

	int selectedItem = 0;

private:
  std::vector<Ref<Item>> items;
};