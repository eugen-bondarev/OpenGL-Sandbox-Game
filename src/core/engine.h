#pragma once

#include "Linow/Linow.h"

#include "game/game.h"
#include "menu/main_menu.h"

class Engine {
public:
	Engine();
	bool IsRunning() const;
	void BeginFrame();
	void Run();
	void EndFrame();
	~Engine();

private:
	bool resetGame { false };
	Ref<Game> game;
	Ref<MainMenu> mainMenu;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};