#pragma once

#include "werwel/werwel.h"

#include "game/world/blocks.h"
#include "game/tools.h"

class TileMap : public ww::Texture
{
public:
	template <typename... Args>
	TileMap(Vec2 tileSize, Args &&...args) : ww::Texture(std::forward<Args>(args)...)
	{
		amountOfTiles = size / tileSize;
	}

	inline Vec2 GetAmountOfTiles() const
	{
		return amountOfTiles;
	}

	inline Vec2 GetTileSize() const
	{
		return size / amountOfTiles;
	}

private:
	Vec2 amountOfTiles;
};

template <typename T>
class ITileMapDictionary
{
public:
	template <typename... Args>
	void Setup(Args &&...args)
	{
		dictionary = {std::forward<Args>(args)...};
	}

	void Add(T t, Vec2 vec)
	{
		dictionary[t] = vec;
	}

	Vec2 Get(T t) const
	{
		return dictionary.at(t);
	}

public:
	std::map<T, Vec2> dictionary;
};

class BlocksTileMap : public TileMap, public ITileMapDictionary<BlockType>
{
public:
	template <typename... Args>
	BlocksTileMap(Args &&...args) : TileMap(std::forward<Args>(args)...) {}
};

class ToolsTileMap : public TileMap, public ITileMapDictionary<ToolType>
{
public:
	template <typename... Args>
	ToolsTileMap(Args &&...args) : TileMap(std::forward<Args>(args)...) {}
};