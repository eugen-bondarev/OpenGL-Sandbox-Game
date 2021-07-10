#include "map_renderer.h"

#include "tiles.h"

#include "util/imgui_helper.h"

MapRenderer::MapRenderer(const Ref<Map> &map, const Ref<Camera> &camera) : map{map}, camera{camera}
{
	// visibleChunks = map->GetVisibleChunks();
	// lastVisibleChunks = visibleChunks;
	map->GetLastVisibleChunks() = map->GetVisibleChunks();

	pipeline.colorPass = CreateRef<ColorPass>(map->GetVisibleChunks().GetArea() * map->GetAmountOfChunks().x * map->GetAmountOfChunks().y);
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
	MW_PROFILER_SCOPE();

	blocksData.clear();
	wallsData.clear();
	lightData.clear();

	static Vec2 offset = {-4.0f, -4.0f};

	BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);

	for (int chunkX = map->GetVisibleChunks().x.start; chunkX < map->GetVisibleChunks().x.end; chunkX++)
	{
		for (int chunkY = map->GetVisibleChunks().y.start; chunkY < map->GetVisibleChunks().y.end; chunkY++)
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
	MW_PROFILER_SCOPE();

	// pipeline.colorPass->GetBlocksVBO()->Store(blocksData);
	pipeline.colorPass->GetBlocksVBO()->Store(renderData);
	pipeline.colorPass->GetWallsVBO()->Store(wallsData);

	std::vector<Vec2> copy = lightData;
	for (const Vec2 &light : additionalLightData)
	{
		copy.push_back(light);
	}

	pipeline.lightPass->GetVBO()->Store(copy);
	MW_SYNC_GPU();
}

void MapRenderer::CheckVisibleChunks()
{
	auto& visibleChunks = map->GetVisibleChunks();
	auto& lastVisibleChunks = map->GetLastVisibleChunks();

	if (visibleChunks != lastVisibleChunks)
	{
		if (lastVisibleChunks.x.end < visibleChunks.x.end)
		{
			int oldX = visibleChunks.x.start;
			int newX = visibleChunks.x.end - 1;

			for (int y = visibleChunks.y.start - 1; y < visibleChunks.y.end + 1; y++)
			{
				auto& data = chunkData[Vec2(oldX, y)];

				// if (data.start == 0)
				// {
				// 	MW_LOG_OUT("NOT FOUND: " << oldX << ' ' << y);
				// }

				Vec2 chunkPosition = Vec2(newX, y) * Vec2(8, 8) * 16.0f;
				
				std::vector<Vec4> bls(64);
				int b = 0;

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						Vec2 blockPosition = chunkPosition + Vec2(i, j) * 16.0f;
						bls[b++] = WhatBlock(blockPosition.x, blockPosition.y);
					}
				}
				
				b = 0;
				for (int i = data.start; i < data.start + data.howMany; i++)
				{
					renderData[i] = bls[b++];
				}

				chunkData[Vec2(newX, y)] = data;
				chunkData.erase(Vec2(oldX, y));
			}
		}
		else if (lastVisibleChunks.x.end > visibleChunks.x.end)
		{
			int oldX = lastVisibleChunks.x.end - 1;
			int newX = lastVisibleChunks.x.start;

			for (int y = visibleChunks.y.start - 1; y < visibleChunks.y.end + 1; y++)
			{
				auto& data = chunkData[Vec2(oldX, y)];

				// if (data.start == 0)
				// {
				// 	MW_LOG_OUT("NOT FOUND: " << oldX << ' ' << y);
				// }

				Vec2 chunkPosition = Vec2(newX, y) * Vec2(8, 8) * 16.0f;
				
				std::vector<Vec4> bls(64);
				int b = 0;

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						Vec2 blockPosition = chunkPosition + Vec2(i, j) * 16.0f;
						bls[b++] = WhatBlock(blockPosition.x, blockPosition.y);
					}
				}
				
				b = 0;
				for (int i = data.start; i < data.start + data.howMany; i++)
				{
					renderData[i] = bls[b++];
				}

				chunkData[Vec2(newX, y)] = data;
				chunkData.erase(Vec2(oldX, y));
			}
		}
		if (lastVisibleChunks.y.start < visibleChunks.y.start)
		{
			int oldY = visibleChunks.y.start;
			int newY = visibleChunks.y.end - 1;

			for (int x = visibleChunks.x.start - 1; x < visibleChunks.x.end + 1; x++)
			{
				auto& data = chunkData[Vec2(x, oldY)];

				// if (data.start == 0)
				// {
				// 	MW_LOG_OUT("NOT FOUND: " << x << ' ' << oldY);
				// }

				Vec2 chunkPosition = Vec2(x, newY) * Vec2(8, 8) * 16.0f;
				
				std::vector<Vec4> bls(64);
				int b = 0;

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						Vec2 blockPosition = chunkPosition + Vec2(i, j) * 16.0f;
						bls[b++] = WhatBlock(blockPosition.x, blockPosition.y);
					}
				}
				
				b = 0;
				for (int i = data.start; i < data.start + data.howMany; i++)
				{
					renderData[i] = bls[b++];
				}

				chunkData[Vec2(x, newY)] = data;
				chunkData.erase(Vec2(x, oldY));
			}
		} 
		else if (lastVisibleChunks.y.start > visibleChunks.y.start)
		{
			int oldY = lastVisibleChunks.y.end - 1;
			int newY = lastVisibleChunks.y.start;

			for (int x = visibleChunks.x.start - 1; x < visibleChunks.x.end + 1; x++)
			{
				auto& data = chunkData[Vec2(x, oldY)];

				// if (data.start == 0)
				// {
				// 	MW_LOG_OUT("NOT FOUND: " << x << ' ' << oldY);
				// }

				Vec2 chunkPosition = Vec2(x, newY) * Vec2(8, 8) * 16.0f;
				
				std::vector<Vec4> bls(64);
				int b = 0;

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						Vec2 blockPosition = chunkPosition + Vec2(i, j) * 16.0f;
						bls[b++] = WhatBlock(blockPosition.x, blockPosition.y);
					}
				}
				
				b = 0;
				for (int i = data.start; i < data.start + data.howMany; i++)
				{
					renderData[i] = bls[b++];
				}

				chunkData[Vec2(x, newY)] = data;
				chunkData.erase(Vec2(x, oldY));
			}
		}

		map->GetLastVisibleChunks() = map->GetVisibleChunks();
		map->chunksUpdated = true;
	}
}

void MapRenderer::PerformRenderPasses(const std::vector<Ref<IRenderer>> &additionalRenderers)
{
	// pipeline.colorPass->Perform(camera, wallsData.size(), blocksData.size(), additionalRenderers);
	pipeline.colorPass->Perform(camera, wallsData.size(), renderData.size(), additionalRenderers);
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