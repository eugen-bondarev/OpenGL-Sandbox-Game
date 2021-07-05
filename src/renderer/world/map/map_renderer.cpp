#include "map_renderer.h"

#include "tiles.h"

#include "util/imgui_helper.h"

MapRenderer::MapRenderer(const Ref<Map> &map, const Ref<Camera> &camera) : map{map}, camera{camera}
{
	visibleChunks = map->GetVisibleChunks();
	lastVisibleChunks = visibleChunks;

	pipeline.colorPass = CreateRef<ColorPass>(visibleChunks.GetArea() * map->GetAmountOfChunks().x * map->GetAmountOfChunks().y);
	pipeline.lightPass = CreateRef<LightPass>();
	pipeline.compositionPass = CreateRef<CompositionPass>();

	PrepareTiles();
}

void MapRenderer::PrepareTile(TilePos tilePos, int x, int y, BlocksTileMap *blocksTileMap)
{
	if (tilePos == TilePos::Foreground)
	{
		if (!map->BlockIsEmpty(x, y))
		{
			const TileFunction &Function = PickTileFunction(map->GetBlocks()[x][y].type);

			const Vec2 tile = blocksTileMap->Get(map->GetBlocks()[x][y].type) + Function(map->GetBlocks(), x, y);
			map->GetBlocks()[x][y].texture = tile;
		}
	}
	else
	{
		if (!map->WallIsEmpty(x, y))
		{
			const Vec2 tile = blocksTileMap->Get(map->GetWalls()[x][y].type == WallType::Grass ? WallType::Dirt : map->GetWalls()[x][y].type) + PickRightAngularWall(map->GetWalls(), x, y) + Vec2(3, 0);
			map->GetWalls()[x][y].texture = tile;
		}
	}
}

void MapRenderer::PrepareTiles()
{
	BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);

	for (int x = 0; x < map->GetBlocks().size(); x++)
	{
		for (int y = 0; y < map->GetBlocks()[x].size(); y++)
		{
			PrepareTile(TilePos::Foreground, x, y, blocksTileMap);
			PrepareTile(TilePos::Background, x, y, blocksTileMap);
		}
	}
}

void MapRenderer::RebuildScene()
{
	NF_PROFILER_SCOPE();

	blocksData.clear();
	wallsData.clear();
	lightData.clear();

	static Vec2 offset = {-4.0f, -4.0f};

	BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);

	for (int chunkX = visibleChunks.x.start; chunkX < visibleChunks.x.end; chunkX++)
	{
		for (int chunkY = visibleChunks.y.start; chunkY < visibleChunks.y.end; chunkY++)
		{
			for (int x = chunkX * map->GetChunkSize().x; x < (chunkX + 1) * map->GetChunkSize().y; x++)
			{
				for (int y = chunkY * map->GetChunkSize().y; y < (chunkY + 1) * map->GetChunkSize().y; y++)
				{
					if (!map->BlockIsEmpty(x, y))
					{
						blocksData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize(), map->GetBlocks()[x][y].texture.x, map->GetBlocks()[x][y].texture.y);

						if (x > 1 && y > 0)
						{
							if (!map->WallIsEmpty(x, y))
							{
								if ((!map->BlockIs(x, y, map->GetBlocks()[x - 1][y].type)) || (!map->BlockIs(x, y, map->GetBlocks()[x][y - 1].type)) || (!map->BlockIs(x, y, map->GetBlocks()[x - 1][y - 1].type)))
								{
									wallsData.emplace_back(x * map->GetBlockSize() + offset.x, y * map->GetBlockSize() + offset.y, map->GetWalls()[x][y].texture.x, map->GetWalls()[x][y].texture.y);
								}
							}
						}
					}
					else
					{
						if (!map->WallIsEmpty(x, y))
						{
							if (map->BlockIsEmpty(x, y + 6) && map->WallIsEmpty(x, y + 6))
							{
								lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
							}

							if (map->BlockIsEmpty(x + 6, y) && map->WallIsEmpty(x + 6, y))
							{
								lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
							}

							if (map->BlockIsEmpty(x - 10, y) && map->WallIsEmpty(x - 10, y))
							{
								lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
							}

							wallsData.emplace_back(x * map->GetBlockSize() + offset.x, y * map->GetBlockSize() + offset.y, map->GetWalls()[x][y].texture.x, map->GetWalls()[x][y].texture.y);
						}
						else
						{
							if (!map->BlockIsEmpty(x, y - 1) || !map->WallIsEmpty(x, y - 1))
							{
								lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
							}

							if (!map->BlockIsEmpty(x, y + 1) || !map->WallIsEmpty(x, y - 1))
							{
								lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
							}

							if (!map->BlockIsEmpty(x - 1, y) || !map->WallIsEmpty(x - 1, y))
							{
								lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
							}

							if (!map->BlockIsEmpty(x + 1, y) || !map->WallIsEmpty(x + 1, y))
							{
								lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
							}
						}
					}
				}
			}
		}
	}
}

void MapRenderer::UpdateScene()
{
	NF_PROFILER_SCOPE();

	pipeline.colorPass->GetBlocksVBO()->Store(blocksData);
	pipeline.colorPass->GetWallsVBO()->Store(wallsData);

	std::vector<Vec2> copy = lightData;
	for (const Vec2 &light : additionalLightData)
	{
		copy.push_back(light);
	}

	pipeline.lightPass->GetVBO()->Store(copy);
	NF_SYNC_GPU();
}

void MapRenderer::CheckVisibleChunks()
{
	visibleChunks = map->GetVisibleChunks();

	if (visibleChunks != lastVisibleChunks)
	{
		lastVisibleChunks = visibleChunks;
		map->chunksUpdated = true;
	}
}

void MapRenderer::PerformRenderPasses(const std::vector<Ref<IRenderer>> &additionalRenderers)
{
	pipeline.colorPass->Perform(camera, wallsData.size(), blocksData.size(), additionalRenderers);
	pipeline.lightPass->Perform(camera, lightData.size() + additionalLightData.size());
}

void MapRenderer::Compose()
{
	pipeline.compositionPass->Perform(pipeline.colorPass, pipeline.lightPass);
}

void MapRenderer::Render(const std::vector<Ref<IRenderer>> &additionalRenderers)
{
	CheckVisibleChunks();

	if (map->blockToUpdate != Vec2(-1))
	{
		BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);

		PrepareTile(TilePos::Foreground, map->blockToUpdate.x - 1, map->blockToUpdate.y, blocksTileMap);
		PrepareTile(TilePos::Foreground, map->blockToUpdate.x + 1, map->blockToUpdate.y, blocksTileMap);
		PrepareTile(TilePos::Foreground, map->blockToUpdate.x, map->blockToUpdate.y, blocksTileMap);
		PrepareTile(TilePos::Foreground, map->blockToUpdate.x, map->blockToUpdate.y - 1, blocksTileMap);
		PrepareTile(TilePos::Foreground, map->blockToUpdate.x, map->blockToUpdate.y + 1, blocksTileMap);

		map->blockToUpdate = Vec2(-1);
	}

	if (map->wallToUpdate != Vec2(-1))
	{
		BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);

		PrepareTile(TilePos::Background, map->wallToUpdate.x - 1, map->wallToUpdate.y, blocksTileMap);
		PrepareTile(TilePos::Background, map->wallToUpdate.x + 1, map->wallToUpdate.y, blocksTileMap);
		PrepareTile(TilePos::Background, map->wallToUpdate.x, map->wallToUpdate.y, blocksTileMap);
		PrepareTile(TilePos::Background, map->wallToUpdate.x, map->wallToUpdate.y - 1, blocksTileMap);
		PrepareTile(TilePos::Background, map->wallToUpdate.x, map->wallToUpdate.y + 1, blocksTileMap);

		map->wallToUpdate = Vec2(-1);
	}

	if (map->blocksUpdated)
	{
		if (map->chunksUpdated)
		{
			RebuildScene();
			map->chunksUpdated = false;
		}
		UpdateScene();
		PerformRenderPasses(additionalRenderers);
	}
	Compose();

	additionalLightData.clear();

	map->blocksUpdated = false;
	map->chunksUpdated = false;
}