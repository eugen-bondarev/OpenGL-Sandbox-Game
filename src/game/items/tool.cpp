#include "tool.h"

#include "game/ecs/player.h"

Tool::Tool(ToolType type, int amount) : type{type}, Item(1, amount)
{
	TYPE = static_cast<int>(type);
}

void Tool::Use(GameState state)
{
	Map::BlockSettingData settingBlock = state.world->GetMap()->PlaceBlock(state.world->GetCamera()->GetPosition(), BlockType::Empty);

	if (settingBlock.IsSet())
	{
		state.player->GetInventory().GiveItem<Block>(settingBlock.oldBlock, 1);
	}
}

ToolType Tool::GetToolType() const
{
	return type;
}