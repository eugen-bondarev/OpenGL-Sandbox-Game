#pragma once

#include "Game/World.h"
#include "Game/Control/Camera.h"

#include "Renderer/Characters/CharacterRenderer.h"

#include "Renderer/DebugRenderer.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

#include "Renderer/MapRenderer.h"

#define LINOW_USE_GLM
#include "Linow/Linow.h"

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
	void PopulateBlockData();

	Ref<Map> map;
	Ref<MapRenderer> mapRenderer;

	void OnVisibleChunksChange();
	Ref<Camera> camera;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};