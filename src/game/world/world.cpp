#include "world.h"

World::World(int seed)
{
	// Terraria - 1680x480
	map = CreateRef<Map>(seed, Vec2(8), Vec2(500));
	camera = CreateRef<Camera>();
	camera->SetPosition((map->GetCenter() - Vec2(1, 0)) * map->GetBlockSize());
	map->CalculateVisibleChunks(camera->GetPosition());

	map->BLOCKS.resize((map->GetVisibleChunks().x.end - map->GetVisibleChunks().x.start) * map->GetChunkSize().x);
	for (int i = 0; i < map->BLOCKS.size(); i++)
	{
		map->BLOCKS[i].resize((map->GetVisibleChunks().y.end - map->GetVisibleChunks().y.start) * map->GetChunkSize().y);
	}

	map->WALLS.resize((map->GetVisibleChunks().x.end - map->GetVisibleChunks().x.start) * map->GetChunkSize().x);
	for (int i = 0; i < map->WALLS.size(); i++)
	{
		map->WALLS[i].resize((map->GetVisibleChunks().y.end - map->GetVisibleChunks().y.start) * map->GetChunkSize().y);
	}

	ConvertChunksRenderData(map.get(), renderData);

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