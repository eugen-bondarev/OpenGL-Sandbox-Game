#pragma once

#include "Item.h"

class Empty : public Item {
public:
  Empty();

  void Use(GameState state) override;
};