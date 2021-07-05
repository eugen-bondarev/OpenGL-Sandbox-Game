#include "world_renderer.h"

WorldRenderer::WorldRenderer(const Ref<World> &world) : world{world}
{
	mapRenderer = CreateRef<MapRenderer>(world->GetMap(), world->GetCamera());
	woodsRenderer = CreateRef<WoodsRenderer>(world->GetWoods(), world->GetCamera());
}

void WorldRenderer::Render(const std::vector<Ref<IRenderer>> &additionalRenderers)
{
	world->GetCamera()->OnPositionChange([&]()
	{
		world->GetMap()->CalculateVisibleChunks(world->GetCamera()->GetPosition());
		world->GetMap()->blocksUpdated = true;
	});

	mapRenderer->Render(additionalRenderers);
}

Ref<MapRenderer> &WorldRenderer::GetMapRenderer()
{
	return mapRenderer;
}

Ref<WoodsRenderer> &WorldRenderer::GetWoodsRenderer()
{
	return woodsRenderer;
}