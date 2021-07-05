#pragma once

#include "assets/image_asset.h"
#include "mgcwel/werwel.h"
#include "tile_map.h"

enum class TextureAtlasType
{
	Map,
	Tools
};

class TextureAtlas
{
public:
	template <typename T>
	static Ref<T> Add(TextureAtlasType type, Ref<T> texture)
	{
		dictionary[type] = dynamic_cast<TileMap *>(texture.get());
		return texture;
	}

	template <typename T>
	static T *Get(TextureAtlasType type)
	{
		return static_cast<T *>(dictionary[type]);
	}

	static void Clear()
	{
		dictionary.clear();
	}

private:
	inline static std::map<TextureAtlasType, TileMap *> dictionary;
};