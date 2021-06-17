#pragma once

#include "Game/World.h"
#include "Game/Control/Camera.h"

#include "Renderer/Characters/CharacterRenderer.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

#include "Renderer/MapRenderer.h"

#include "Game/Character.h"
#include "Renderer/Characters/CharacterRenderer.h"

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

	Ref<Character> character;
	Ref<CharacterRenderer> characterRenderer;

	void OnVisibleChunksChange();
	Ref<Camera> camera;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};