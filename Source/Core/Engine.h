#pragma once

#include "Game/World.h"
#include "Renderer/WorldRenderer.h"

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
	Ref<Camera> camera;
	Ref<World> world;
	Ref<WorldRenderer> worldRenderer;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};