#pragma once

#include "ItemCell.h"

class Inventory {
friend class Player;
friend class InterfaceRenderer;

public:
  Inventory();

	int selectedItem = 0;

private:
  std::vector<ItemCell> cells;
};