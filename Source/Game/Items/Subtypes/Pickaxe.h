#pragma once

#include "BlocksDestroyer.h"

class Pickaxe : public BlocksDestroyer {
public:
  NF_INHERIT_CONSTRUCTOR(Pickaxe, BlocksDestroyer) {}
  
  std::vector<BlockType> GetDestroyableBlocks() const override {
    return {
      BlockType::Stone
    };
  }
};