#pragma once

#include "blocks_destroyer.h"

class Shovel : public BlocksDestroyer {
public:
  NF_INHERIT_CONSTRUCTOR(Shovel, BlocksDestroyer) {}
  
  std::vector<BlockType> GetDestroyableBlocks() const override {
    return {
      BlockType::Dirt,
      BlockType::Grass
    };
  }
};