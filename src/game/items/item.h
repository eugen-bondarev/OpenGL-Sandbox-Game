#pragma once

#include "game_state.h"

#include "renderer/atlas/tile_map.h"

using Icon = std::pair<TileMap *, Vec2>;

class Item
{
public:
	Item(int stack = 1, int amount = 1);

	virtual void Use(GameState state) = 0;

	int GetStack() const;
	int GetCurrentAmount() const;
	void SetCurrentAmount(int amount);

	bool IsEmpty() const;

	virtual Icon GetIcon()
	{
		return {
			nullptr,
			Vec2()};
	}

	int TYPE{-1};

protected:
	int stack{0};

	int currentAmount{0};
};