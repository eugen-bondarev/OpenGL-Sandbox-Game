#pragma once

#include "../Blocks.h"

#include "Item.h"

#include "Renderer/Atlas/TextureAtlas.h"

class Wall : public Item {
public:
  Wall(WallType type, int amount = 1);

  void Use(GameState state) override;

  WallType GetWallType() const;

  Icon GetIcon() override {
    return {
      TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map),
      TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map)->Get(type) + Vec2(1, 2) + Vec2(3, 0),
    };
  }

private:
  WallType type;
};