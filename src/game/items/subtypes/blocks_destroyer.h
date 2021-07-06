#pragma once

#include "game/items/tool.h"

class BlocksDestroyer : public Tool
{
public:
	MW_INHERIT_CONSTRUCTOR(BlocksDestroyer, Tool) {}

	virtual void Use(GameState state) override;
	virtual std::vector<BlockType> GetDestroyableBlocks() const = 0;
};