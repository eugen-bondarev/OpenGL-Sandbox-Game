#pragma once

#include "Game/World/Map.h"
#include "Game/Entities/Camera.h"
#include "Tree.h"

class Woods {
public:
  Woods(const Ref<Map>& map);

  const std::vector<Tree>& GetTrees() const;
  void GetVisibleTrees(std::vector<Vec2>& vector, const Ref<Camera>& camera) const;

private:
  std::vector<Tree> trees;

  Woods(const Woods&) = delete;
  Woods& operator=(const Woods&) = delete;
};