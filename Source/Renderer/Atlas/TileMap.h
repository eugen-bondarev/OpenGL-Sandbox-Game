#pragma once

#include "Werwel/Texture.h"

#include "Game/Blocks.h"

class TileMap : public Werwel::Texture {
public:
  template <typename... Args>
  TileMap(Args&&... args) : Werwel::Texture(std::forward<Args>(args)...) {
    amountOfTilesX = static_cast<int>(size.x / 8.0f);
    amountOfTilesY = static_cast<int>(size.y / 8.0f);
  }

  inline int TileMap::GetAmountOfTilesX() const {
    return amountOfTilesX;
  }

  inline int TileMap::GetAmountOfTilesY() const {
    return amountOfTilesY;
  }

private:
  int amountOfTilesX { 0 };
  int amountOfTilesY { 0 };
};

template <typename T>
class ITileMapDictionary {
public:
  template <typename... Args>
  void Setup(Args&&... args) {
    dictionary = { std::forward<Args>(args)... };
  }

  void Add(T t, Vec2 vec) {
    dictionary[t] = vec;
  }

  Vec2 Get(T t) const {
    return dictionary.at(t);
  }

private:
  std::map<T, Vec2> dictionary;
};

class BlocksTileMap : public TileMap, public ITileMapDictionary<BlockType> {
public:
  template <typename... Args>
  BlocksTileMap(Args&&... args) : TileMap(std::forward<Args>(args)...) {}
};