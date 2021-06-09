#pragma once

#include "Game/World.h"
#include "Renderer/WorldRenderer.h"
#include "Game/Character.h"
#include "Renderer/Characters/CharacterRenderer.h"

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
	Ref<Character> character;
	Ref<CharacterRenderer> characterRenderer;

	Ref<Camera> camera;
	Ref<World> world;
	Ref<WorldRenderer> worldRenderer;

	Pos lastDestroyedBlock;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};