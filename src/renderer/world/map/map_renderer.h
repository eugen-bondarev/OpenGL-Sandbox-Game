#pragma once

#include "game/world/map.h"

#include "color_pass/color_pass.h"
#include "light_pass/light_pass.h"
#include "composition_pass/composition_pass.h"

#include "renderer/renderer.h"

class MapRenderer
{
public:
	std::vector<Vec4> sortedBlocks;
	std::vector<Vec4> sortedWalls;
	std::vector<Vec2> sortedLights;

	MapRenderer(const Ref<Camera> &camera);

	void UpdateScene();

	void CheckVisibleChunks();
	void PerformRenderPasses(const std::vector<Ref<IRenderer>> &additionalRenderers);
	void Compose();

	void Render(const std::vector<Ref<IRenderer>> &additionalRenderers);

	inline int GetAmountOfRenderedBlocks() const
	{
		return blocksData.size();
	}

	inline int GetAmountOfRenderedWalls() const
	{
		return wallsData.size();
	}

	inline int GetAmountOfRenderedLights() const
	{
		return lightData.size();
	}

	inline std::vector<Vec2> &GetAdditionalLightData()
	{
		return additionalLightData;
	}

private:
	struct
	{
		Ref<ColorPass> colorPass;
		Ref<LightPass> lightPass;
		Ref<CompositionPass> compositionPass;
	} pipeline;

	std::vector<Vec2> additionalLightData;

	std::vector<BlockData> blocksData;
	std::vector<WallData> wallsData;
	std::vector<Vec2> lightData;

	const Ref<Camera> &camera;

	MapRenderer(const MapRenderer &) = delete;
	MapRenderer &operator=(const MapRenderer &) = delete;
};