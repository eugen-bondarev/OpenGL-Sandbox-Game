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

#include "Linow/Linow.h"

#include "Game/Game.h"

class Engine {
public:
	Engine();
	bool IsRunning() const;
	void BeginFrame();
	void Render();
	void EndFrame();
	~Engine();

private:
	Ptr<Game> game;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};