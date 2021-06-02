#pragma once

#include "GPU/Shader.h"

#include "Game/Map.h"

#include "Renderer/Terrain/ColorPass.h"
#include "Renderer/Terrain/LightPass.h"
#include "Renderer/Terrain/CompositionPass.h"
#include "Renderer/Entities/Sprite.h"

class Engine {
public:
	Engine();
	void InitResources();
	bool IsRunning() const;
	void BeginFrame();
	void Control();
	void Render();
	void EndFrame();
	~Engine();

private:
	Ref<Map> map;

	bounds_t visibleBlocks;
	bounds_t lastVisibleBlocks;
	bool chunksChanged { false };

	struct {
		Ref<ColorPass> color;
		Ref<LightPass> light;
		Ref<CompositionPass> composition;
	} pipeline;

	struct {
		Mat4 matrix;
		Vec2 position;
		Vec2 lastPosition;
	} view;

	bool rerender { true };
};