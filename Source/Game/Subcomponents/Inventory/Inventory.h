#pragma once

#include "ItemCell.h"

class Inventory {
friend class Player;

public:
  Inventory();

private:
  std::vector<ItemCell> cells;
};