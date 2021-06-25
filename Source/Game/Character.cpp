#include "Character.h"

Character::Character(const Ref<Map>& map) : map { map } {
  AddComponent<Animator>();
  AddComponent<Rigidbody>(map);
}

void Character::CollectLights(std::vector<Vec2>& lights) const {
  FORGIO_PROFILER_SCOPE();
  
  const auto& blocks = map->GetBlocks();
  const auto& walls = map->GetWalls();

  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      auto& block = rigidbody->GetBlockNearby(Vec2(), Vec2(1 + x, 2 + y * 2));
      if (blocks[block.index.x][block.index.y] == BlockType::Empty && walls[block.index.x][block.index.y] == WallType::Empty) {
        lights.push_back(block.worldPosition);
      }
    }
  }
}