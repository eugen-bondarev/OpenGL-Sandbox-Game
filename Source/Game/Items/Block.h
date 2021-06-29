#pragma once

#include "../Blocks.h"

#include "Item.h"

#include "Renderer/Atlas/TextureAtlas.h"

class Block : public Item {
public:
  Block(BlockType type, int amount = 1);

  void Use(GameState state) override;

  BlockType GetBlockType() const;

  Icon GetIcon() override {
    return {
      TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map),
      TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map)->Get(type) + Vec2(1, 2),
    };
  }

private:
  BlockType type;
};