#pragma once

#include "Map.h"
#include "Woods/Woods.h"
#include "Game/Entities/Camera.h"

class World {
public:
  World(int seed);

  Ref<Map>& GetMap();
  Ref<Camera>& GetCamera();
  Ref<Woods>& GetWoods();

private:
	Ref<Map> map;
	Ref<Camera> camera;

  Ref<Woods> woods;

  World(const World&) = delete;
  World& operator=(const World&) = delete;
};