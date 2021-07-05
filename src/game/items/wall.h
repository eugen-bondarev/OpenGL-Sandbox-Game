#pragma once

#include "game/world/blocks.h"

#include "item.h"

#include "renderer/atlas/texture_atlas.h"

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