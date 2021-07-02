#include "Woods.h"

#include "Core/Window.h"

Woods::Woods(const Ref<Map>& map) {
  for (int x = 0; x < map->GetBlocks().size(); x++) {
    for (int y = 0; y < map->GetBlocks()[x].size(); y++) {
      if (!map->BlockIs(x, y, BlockType::Grass)) {
        continue;
      }

      if (rand() % 100 < 5) {
        trees.emplace_back(Vec2 { x * map->GetBlockSize(), (y + 6.55f) * map->GetBlockSize() });
      }
    }
  }
}

const std::vector<Tree>& Woods::GetTrees() const {
  return trees;
}

void Woods::GetVisibleTrees(std::vector<Vec2>& vector, const Ref<Camera>& camera) const {
  int tree = 0;

  for (int i = 0; i < trees.size(); i++) {
    if (glm::distance(trees[i].GetPosition(), camera->GetPosition()) < 500) {
      tree = i;
      break;
    }
  }

  // To let them appear when they're still invisible
  static float offset = 350.0f;

  for (int i = tree; i < std::min<int>(tree + 25, trees.size()); i++) {
    if (Math::Distance(trees[i].GetPosition(), camera->GetPosition()) < (NF_WINDOW_WIDTH() + offset) / 2.0f) {
      vector.push_back(trees[i].GetPosition());
    }
  }

  for (int i = tree - 1; i > std::max<int>(tree - 25, 0); i--) {    
    if (Math::Distance(trees[i].GetPosition(), camera->GetPosition()) < (NF_WINDOW_WIDTH() + offset) / 2.0f) {
      vector.push_back(trees[i].GetPosition());
    }
  }
}