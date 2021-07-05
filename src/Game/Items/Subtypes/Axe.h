#pragma once

#include "BlocksDestroyer.h"

class Axe : public BlocksDestroyer {
public:
  NF_INHERIT_CONSTRUCTOR(Axe, BlocksDestroyer) {}
  
  std::vector<BlockType> GetDestroyableBlocks() const override {
    return {
      BlockType::Wood
    };
  }
};