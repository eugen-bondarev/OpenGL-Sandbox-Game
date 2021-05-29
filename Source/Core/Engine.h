#pragma once

#include "Gpu/Shader.h"
#include "Gpu/ColorFbo.h"
#include "Gpu/LightFbo.h"

#include "Game/Map.h"

#include "Renderer/ColorPass.h"
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
	struct {
		std::shared_ptr<ColorPass> color;
		std::shared_ptr<CompositionPass> composition;
	} pipeline;

	std::shared_ptr<Map> map;

	struct {
		Mat4 matrix;
		Vec2 position;
		Vec2 lastPosition;
	} view;

	bool rerender { true };
};