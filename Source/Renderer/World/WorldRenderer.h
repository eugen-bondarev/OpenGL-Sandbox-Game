#pragma once

#include "Game/World/World.h"

#include "Map/MapRenderer.h"
#include "Woods/WoodsRenderer.h"

#include "Renderer/Renderer.h"

class WorldRenderer {
public:
  WorldRenderer(const Ref<World>& world);

  void Render(const std::vector<Ref<IRenderer>> &additionalRenderers);

  Ref<MapRenderer>& GetMapRenderer();
  Ref<WoodsRenderer>& GetWoodsRenderer();

private:
  const Ref<World>& world;

  Ref<MapRenderer> mapRenderer;
  Ref<WoodsRenderer> woodsRenderer;

  WorldRenderer(const WorldRenderer&) = delete;
  WorldRenderer& operator=(const WorldRenderer&) = delete;
};