#include "wall.h"

Wall::Wall(WallType type, int amount) : type{type}, Item(64, amount)
{
	TYPE = static_cast<int>(type);
}

void Wall::Use(GameState state)
{
	// Map::BlockSettingData settingBlock = state.world->GetMap()->PlaceWall(state.world->GetCamera()->GetPosition(), type);

	// if (settingBlock.IsSet())
	// {
	// 	currentAmount -= 1;
	// }
}

WallType Wall::GetWallType() const
{
	return type;
}