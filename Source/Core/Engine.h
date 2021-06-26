#pragma once

#include "Linow/Linow.h"

#include "Game/Game.h"
#include "Menu/MainMenu.h"

class Engine {
public:
	Engine();
	bool IsRunning() const;
	void BeginFrame();
	void Render();
	void EndFrame();
	~Engine();

private:
	Ref<Game> game;
	Ref<MainMenu> mainMenu;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};