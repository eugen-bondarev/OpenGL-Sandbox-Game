#pragma once

#include "game/world/world.h"

class Player;

struct GameState
{
	GameState(World *world = nullptr, Player *player = nullptr);

	World *world{nullptr};
	Player *player{nullptr};
};