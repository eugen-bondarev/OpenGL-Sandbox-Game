#include "WorldRenderer.h"

WorldRenderer::WorldRenderer(const Ref<World>& world) : world { world } {
	mapRenderer = CreateRef<MapRenderer>(world->GetMap(), world->GetCamera());
}

void WorldRenderer::Render(const std::vector<Ref<IRenderer>> &additionalRenderers) {
	world->GetCamera()->OnPositionChange([&]() {
		world->GetMap()->CalculateVisibleChunks(world->GetCamera()->GetPosition());
		world->GetMap()->blocksUpdated = true;
	});

  mapRenderer->Render(additionalRenderers);
}

Ref<MapRenderer>& WorldRenderer::GetMapRenderer() {
  return mapRenderer;
}