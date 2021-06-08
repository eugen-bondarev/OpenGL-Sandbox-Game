#pragma once

#include "Map.h"

class World {
public:
  World();

  inline Ref<Map>& GetMap() {
    return map;
  }

private:
  Ref<Map> map;

  World(const World&) = delete;
  World& operator=(const World&) = delete;
};