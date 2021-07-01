#pragma once

#include "Map.h"
#include "Game/Entities/Camera.h"

class World {
public:
  World(int seed);

  inline Ref<Map>& GetMap() {
    return map;
  }

  inline Ref<Camera>& GetCamera() {
    return camera;
  }

private:
	Ref<Map> map;
	Ref<Camera> camera;

  World(const World&) = delete;
  World& operator=(const World&) = delete;
};