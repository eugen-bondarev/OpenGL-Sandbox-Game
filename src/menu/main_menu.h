#pragma once

#include "game/game.h"

enum class MenuLocation
{
	Main = 0,
	Settings
};

class MainMenu
{
public:
	MainMenu(Ref<Game> &game);

	void Show();

private:
	Ref<Game> &game;

	void Settings();
	void Main();

	MenuLocation location{MenuLocation::Main};
};