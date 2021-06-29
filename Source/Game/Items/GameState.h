#pragma once

#include "../World.h"

struct GameState {
  GameState(World* world = nullptr);

  World* world { nullptr };
};