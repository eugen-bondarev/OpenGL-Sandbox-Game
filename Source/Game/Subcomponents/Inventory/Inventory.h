#pragma once

#include "ItemCell.h"

class Inventory {
friend class Player;
friend class InterfaceRenderer;

public:
  Inventory();

private:
  std::vector<ItemCell> cells;
};