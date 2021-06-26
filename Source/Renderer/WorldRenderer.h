#pragma once

#include "Game/World.h"

#include "MapRenderer.h"

#include "Renderer.h"

class WorldRenderer {
public:
  WorldRenderer(const Ref<World>& world);

  void Render(const std::vector<Ref<IRenderer>> &additionalRenderers);

  Ref<MapRenderer>& GetMapRenderer();

private:
  const Ref<World>& world;

  Ref<MapRenderer> mapRenderer;

  WorldRenderer(const WorldRenderer&) = delete;
  WorldRenderer& operator=(const WorldRenderer&) = delete;
};