#pragma once

#include "BlocksDestroyer.h"

class Shovel : public BlocksDestroyer {
public:
  NATURAFORGE_INHERIT_CONSTRUCTOR(Shovel, BlocksDestroyer) {}
  
  std::vector<BlockType> GetDestroyableBlocks() const override {
    return {
      BlockType::Dirt,
      BlockType::Grass
    };
  }
};