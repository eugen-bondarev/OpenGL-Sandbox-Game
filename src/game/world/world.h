#pragma once

#include "map.h"
#include "woods/woods.h"
#include "game/entities/camera.h"

class World
{
public:
	World(int seed);

	Ref<Map> &GetMap();
	Ref<Camera> &GetCamera();
	Ref<Woods> &GetWoods();

private:
	Ref<Map> map;
	Ref<Camera> camera;

	Ref<Woods> woods;

	World(const World &) = delete;
	World &operator=(const World &) = delete;
};