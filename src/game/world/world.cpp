#include "world.h"

World::World(int seed)
{
	// Terraria - 1680x480
	map = CreateRef<Map>(seed, Vec2(5), Vec2(500));
	camera = CreateRef<Camera>();
	camera->SetPosition((map->GetCenter() - Vec2(1, 0)) * map->GetBlockSize());
	map->CalculateVisibleChunks(camera->GetPosition());

	woods = CreateRef<Woods>(map);
}

Ref<Map> &World::GetMap()
{
	return map;
}

Ref<Camera> &World::GetCamera()
{
	return camera;
}

Ref<Woods> &World::GetWoods()
{
	return woods;
}