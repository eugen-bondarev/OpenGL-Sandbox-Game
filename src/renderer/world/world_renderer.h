#pragma once

#include "game/world/world.h"

#include "map/map_renderer.h"
#include "woods/woods_renderer.h"

#include "renderer/renderer.h"

class WorldRenderer 
{
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