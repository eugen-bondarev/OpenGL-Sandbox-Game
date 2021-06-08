#pragma once

#include "Game/World.h"

#include "Renderer/Terrain/ColorPass.h"
#include "Renderer/Terrain/LightPass.h"
#include "Renderer/Terrain/CompositionPass.h"

#include "Game/Control/Camera.h"

class WorldRenderer {
public:
  WorldRenderer(Ref<World>& world, Ref<Camera>& camera);

  void Render();

	bounds_t visibleBlocks;
	bounds_t lastVisibleBlocks;

	struct {
		Ref<ColorPass> color;
		Ref<LightPass> light;
		Ref<CompositionPass> composition;
    bool rerender;
	} mapPipeline;

  inline void ForceRerender() {
    mapPipeline.rerender = true;
  }	

private:
  Ref<World> world;
	Ref<Camera> camera;

	bool chunksChanged { false };

  WorldRenderer(const WorldRenderer&) = delete;
  WorldRenderer& operator=(const WorldRenderer&) = delete;
};