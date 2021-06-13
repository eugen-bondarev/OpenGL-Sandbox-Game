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

	std::vector<Vec4> blocksData;
	void PopulateBlockData(bool firstTime);

	Ref<Map> map;
	Ref<Werwel::Texture> tileMap;
	Ref<Werwel::Shader> shader;
	Ref<Werwel::VAO> vao;
	Ref<Werwel::VBO> vbo;

	bounds_t lastVisibleChunks;
	bounds_t visibleChunks;
	
	bool leftNew 		{ false };
	bool leftGone 	{ false };
	bool rightNew 	{ false };
	bool rightGone 	{ false };
	bool topNew 		{ false };
	bool topGone 		{ false };
	bool botNew 		{ false };
	bool botGone 		{ false };

	void OnVisibleChunksChange();
	Ref<Camera> camera;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};