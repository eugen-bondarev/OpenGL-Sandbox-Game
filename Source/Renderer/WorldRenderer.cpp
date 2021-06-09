#include "WorldRenderer.h"

WorldRenderer::WorldRenderer(Ref<World>& world, Ref<Camera>& camera) {
  this->world = world;
  this->camera = camera;

	visibleBlocks = world->GetMap()->GetVisibleChunks();
	lastVisibleBlocks = world->GetMap()->GetVisibleChunks();

	mapPipeline.color = CreateRef<ColorPass>(world->GetMap());
	mapPipeline.light = CreateRef<LightPass>(mapPipeline.color->GetMapRenderer());
	mapPipeline.composition = CreateRef<CompositionPass>();

	camera->SetPosition(world->GetMap()->GetCenter() * world->GetMap()->GetBlockSize());	
	world->GetMap()->CalculateVisibleChunks(camera->GetPosition());
}

void WorldRenderer::Render(std::function<void()> add, const std::vector<Pos>& additionalLightData) {
	if (camera->GetLastPosition() != camera->GetPosition()) {
		mapPipeline.rerender = true;
		world->GetMap()->CalculateVisibleChunks(camera->GetPosition());
		visibleBlocks = world->GetMap()->GetVisibleChunks();
		camera->SetLastPosition(camera->GetPosition());
	}

	if (lastVisibleBlocks != visibleBlocks) {
		chunksChanged = true;
		lastVisibleBlocks = visibleBlocks;
	}

	if (mapPipeline.rerender) {
		mapPipeline.color->Execute(camera->GetViewMatrix(), camera->GetPosition(), add);

		for (int i = 0; i < additionalLightData.size(); i++) {
			mapPipeline.color->GetLightData().push_back(additionalLightData[i]);
		}

		mapPipeline.light->Execute(camera->GetViewMatrix(), camera->GetPosition(), mapPipeline.color->GetLightData());
		mapPipeline.rerender = false;
	}

	mapPipeline.composition->Execute(mapPipeline.color, mapPipeline.light);

	chunksChanged = false;
}
