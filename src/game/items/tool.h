#pragma once

#include "../tools.h"

#include "item.h"

#include "renderer/atlas/texture_atlas.h"

class Tool : public Item
{
public:
	Tool(ToolType type, int amount = 1);

	void Use(GameState state) override;

	ToolType GetToolType() const;

	Icon GetIcon() override
	{
		return {
			TextureAtlas::Get<ToolsTileMap>(TextureAtlasType::Tools),
			TextureAtlas::Get<ToolsTileMap>(TextureAtlasType::Tools)->Get(type),
		};
	}

private:
	ToolType type;
};