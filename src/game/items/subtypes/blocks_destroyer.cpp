#include "blocks_destroyer.h"

#include "game/ecs/player.h"

#include "util/vector.h"

void BlocksDestroyer::Use(GameState state)
{
	BlockType blockUnderCursor = state.world->GetMap()->GetBlockUnderCursor(state.world->GetCamera()->GetPosition());

	if (Vector::Contains(GetDestroyableBlocks(), blockUnderCursor))
	{
		Map::BlockSettingData settingBlock = state.world->GetMap()->PlaceBlock(state.world->GetCamera()->GetPosition(), BlockType::Empty);

		if (settingBlock.IsSet())
		{
			state.player->GetInventory().GiveItem<Block>(settingBlock.oldBlock, 1);
		}
	}
}