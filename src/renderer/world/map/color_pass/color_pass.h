#pragma once

#include "color_fbo.h"

#include "werwel/werwel.h"

#include "game/entities/camera.h"

#include "renderer/atlas/texture_atlas.h"

#include "renderer/renderer.h"

struct BlockData
{
	struct
	{
		float x{0}, y{0};
	} pos;

	struct
	{
		float x{0}, y{0};
	} tile;

	BlockData() = default;
	BlockData(float posX, float posY, float tileX, float tileY)
	{
		pos.x = posX;
		pos.y = posY;
		tile.x = tileX;
		tile.y = tileY;
	}
};

using WallData = BlockData;

class ColorPass
{
public:
	ColorPass(int amountOfBlocks);

	void Perform(const Ref<Camera> &camera, int amountOfWalls, int amountOfBlocks, const std::vector<Ref<IRenderer>> &additionalRenderers = {});

	inline const Ref<ww::VBO> &GetBlocksVBO() const
	{
		return blocks.vbo;
	}

	inline const Ref<ww::VBO> &GetWallsVBO() const
	{
		return walls.vbo;
	}

	inline const Ref<ColorFBO> &GetFBO() const
	{
		return fbo;
	}

private:
	Ref<ColorFBO> fbo;
	Ref<ww::Shader> shader;

	struct
	{
		Ref<ww::VAO> vao;
		Ref<ww::VBO> vbo;
	} blocks;

	struct
	{
		Ref<ww::VAO> vao;
		Ref<ww::VBO> vbo;
	} walls;

	Ref<BlocksTileMap> tileMap;
};