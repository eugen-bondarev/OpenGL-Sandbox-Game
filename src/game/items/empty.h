#pragma once

#include "item.h"

class Empty : public Item {
public:
  Empty();

  void Use(GameState state) override;
};