#pragma once

#include "blocks_destroyer.h"

class Pickaxe : public BlocksDestroyer
{
public:
	MW_INHERIT_CONSTRUCTOR(Pickaxe, BlocksDestroyer) {}

	std::vector<BlockType> GetDestroyableBlocks() const override
	{
		return { BlockType::Stone };
	}
};