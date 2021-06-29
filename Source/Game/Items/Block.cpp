#include "Block.h"

Block::Block(BlockType type, int amount) : type { type }, Item(256, amount) {
  TYPE = static_cast<int>(type);  
}

void Block::Use(GameState state) {
  Map::BlockSettingData settingBlock = state.world->GetMap()->PlaceBlock(state.world->GetCamera()->GetPosition(), type);

  if (settingBlock.IsSet()) {
    currentAmount -= 1;
  }
}

BlockType Block::GetBlockType() const {
  return type;
}