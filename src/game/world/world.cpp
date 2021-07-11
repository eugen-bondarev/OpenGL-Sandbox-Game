#include "world.h"

World::World(int seed)
{
	// Terraria - 1680x480
	// map = CreateRef<Map>(seed, Vec2(2), Vec2(500));
	Map::Init(seed);
	camera = CreateRef<Camera>();
	camera->SetPosition(0, 0);

	Map::CalculateVisibleChunks(camera->GetPosition());

	Map::Blocks.resize((Map::VisibleChunks.x.end - Map::VisibleChunks.x.start) * Map::GetChunkSize().x);
	for (int i = 0; i < Map::Blocks.size(); i++)
	{
		Map::Blocks[i].resize((Map::VisibleChunks.y.end - Map::VisibleChunks.y.start) * Map::GetChunkSize().y);
	}

	Map::Walls.resize((Map::VisibleChunks.x.end - Map::VisibleChunks.x.start) * Map::GetChunkSize().x);
	for (int i = 0; i < Map::Walls.size(); i++)
	{
		Map::Walls[i].resize((Map::VisibleChunks.y.end - Map::VisibleChunks.y.start) * Map::GetChunkSize().y);
	}

	Map::PopulateVisibleMap();

	woods = CreateRef<Woods>();
}

Ref<Camera> &World::GetCamera()
{
	return camera;
}

Ref<Woods> &World::GetWoods()
{
	return woods;
}