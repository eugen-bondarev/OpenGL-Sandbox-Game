#pragma once

#include "Game/World.h"
#include "Game/Character.h"

#include "Renderer/WorldRenderer.h"
#include "Renderer/Characters/CharacterRenderer.h"

#include "Renderer/DebugRenderer.h"

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
	Ref<DebugRenderer> debugRenderer;

	Ref<Character> character;
	Ref<CharacterRenderer> characterRenderer;

	Ref<Camera> camera;
	Ref<World> world;
	Ref<WorldRenderer> worldRenderer;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};