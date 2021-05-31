#pragma once

#include "Gpu/Shader.h"

#include "Game/Map.h"

#include "Renderer/ColorFbo.h"
#include "Renderer/LightFbo.h"
#include "Renderer/ColorPass.h"
#include "Renderer/LightPass.h"
#include "Renderer/CompositionPass.h"
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
	std::shared_ptr<Map> map;

	struct {
		std::shared_ptr<ColorPass> color;
		std::shared_ptr<LightPass> light;
		std::shared_ptr<CompositionPass> composition;
	} pipeline;

	struct {
		Mat4 matrix;
		Vec2 position;
		Vec2 lastPosition;
	} view;

	bool rerender { true };
};