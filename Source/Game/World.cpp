#include "World.h"

World::World(int seed) {
	map = CreateRef<Map>(seed, Vec2(5, 5), Vec2(500, 500));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());
	map->CalculateVisibleChunks(camera->GetPosition());
}