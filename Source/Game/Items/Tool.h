#pragma once

#include "../Tools.h"

#include "Item.h"

#include "Renderer/Atlas/TextureAtlas.h"

class Tool : public Item {
public:
  Tool(ToolType type, int amount = 1);

  void Use(GameState state) override;

  ToolType GetToolType() const;

  Icon GetIcon() override {
    return {
      TextureAtlas::Get<ToolsTileMap>(TextureAtlasType::Tools),
      TextureAtlas::Get<ToolsTileMap>(TextureAtlasType::Tools)->Get(type),
    };
  }

private:
  ToolType type;
};