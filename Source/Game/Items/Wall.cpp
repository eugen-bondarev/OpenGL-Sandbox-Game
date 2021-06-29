#include "Wall.h"

Wall::Wall(WallType type, int amount) : type { type }, Item(999, amount) {
  
}

void Wall::Use(GameState state) {
  Map::BlockSettingData settingBlock = state.world->GetMap()->PlaceWall(state.world->GetCamera()->GetPosition(), type);

  if (settingBlock.IsSet()) {
    currentAmount -= 1;
  }
}

WallType Wall::GetWallType() const {
  return type;
}