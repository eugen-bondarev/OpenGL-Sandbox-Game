#pragma once

#include "game/world/blocks.h"

#include "item.h"

#include "renderer/atlas/texture_atlas.h"

class Block : public Item
{
public:
	Block(BlockType type, int amount = 1);

	void Use(GameState state) override;

	BlockType GetBlockType() const;

	Icon GetIcon() override
	{
		return {
			TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map),
			TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map)->Get(type) + Vec2(1, 2),
		};
	}

private:
	BlockType type;
};