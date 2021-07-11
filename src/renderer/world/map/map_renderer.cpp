#include "map_renderer.h"

#include "tiles.h"

#include "util/imgui_helper.h"

MapRenderer::MapRenderer(const Ref<Camera> &camera) : camera{camera}
{
	pipeline.colorPass = CreateRef<ColorPass>(0);
	pipeline.lightPass = CreateRef<LightPass>();
	pipeline.compositionPass = CreateRef<CompositionPass>();
}

void MapRenderer::UpdateScene()
{
	MW_PROFILER_SCOPE();

	BlocksTileMap *blocksTileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);

	sortedBlocks.clear();
	sortedWalls.clear();
	sortedLights.clear();

	static Vec2 offset = {-4.0f, -4.0f};

	for (int x = 0; x < Map::Blocks.size(); x++)
	{
		for (int y = 0; y < Map::Blocks[x].size(); y++)
		{
			if (Map::Blocks[x][y].type != BlockType::Empty)
			{
				const TileFunction &Function = PickTileFunction(Map::Blocks[x][y].type);
				const Vec2 blockTextureTile = blocksTileMap->Get(Map::Blocks[x][y].type) + Function(Map::Blocks, x, y);
				sortedBlocks.emplace_back(Map::Blocks[x][y].worldPosition, blockTextureTile);
				
				if (x > 1 && y > 0)
				{
					if (!Map::WallIsEmpty(x, y))
					{
						if ((!Map::BlockIs(x, y, Map::GetBlocks()[x - 1][y].type)) || (!Map::BlockIs(x, y, Map::GetBlocks()[x][y - 1].type)) || (!Map::BlockIs(x, y, Map::GetBlocks()[x - 1][y - 1].type)))
						{
							const Vec2 wallTextureTile = blocksTileMap->Get(Map::Walls[x][y].type == WallType::Grass ? WallType::Dirt : Map::Walls[x][y].type) + PickRightAngularWall(Map::Walls, x, y) + Vec2(3, 0);
							sortedWalls.emplace_back(Map::Walls[x][y].worldPosition + offset, wallTextureTile);
						}
					}
				}
			}
			else
			{				
				if (!Map::WallIsEmpty(x, y))
				{
					if (y + 6 < Map::Blocks[0].size() && Map::BlockIsEmpty(x, y + 6) && Map::WallIsEmpty(x, y + 6))
					{
						sortedLights.push_back(Map::Blocks[x][y].worldPosition);
					}

					if (x + 6 < Map::Blocks.size() && Map::BlockIsEmpty(x + 6, y) && Map::WallIsEmpty(x + 6, y))
					{
						sortedLights.push_back(Map::Blocks[x][y].worldPosition);
					}

					if (x - 10 >= 0 && Map::BlockIsEmpty(x - 10, y) && Map::WallIsEmpty(x - 10, y))
					{
						sortedLights.push_back(Map::Blocks[x][y].worldPosition);
					}

					const Vec2 textureTile = blocksTileMap->Get(Map::Walls[x][y].type == WallType::Grass ? WallType::Dirt : Map::Walls[x][y].type) + PickRightAngularWall(Map::Walls, x, y) + Vec2(3, 0);
					sortedWalls.emplace_back(Map::Walls[x][y].worldPosition + offset, textureTile);
				}
				else
				{					
					if (y > 0 && (!Map::BlockIsEmpty(x, y - 1) || !Map::WallIsEmpty(x, y - 1)))
					{
						sortedLights.push_back(Map::Blocks[x][y].worldPosition);
					}

					if (y > 0 && y + 1 < Map::Blocks[0].size() && (!Map::BlockIsEmpty(x, y + 1) || !Map::WallIsEmpty(x, y - 1)))
					{
						sortedLights.push_back(Map::Blocks[x][y].worldPosition);
					}

					if (x > 0 && (!Map::BlockIsEmpty(x - 1, y) || !Map::WallIsEmpty(x - 1, y)))
					{
						sortedLights.push_back(Map::Blocks[x][y].worldPosition);
					}

					if (x + 1 < Map::Blocks.size() && (!Map::BlockIsEmpty(x + 1, y) || !Map::WallIsEmpty(x + 1, y)))
					{
						sortedLights.push_back(Map::Blocks[x][y].worldPosition);
					}
				}
			}
		}
	}

	for (const Vec2& light : additionalLightData)
	{
		sortedLights.push_back(light);
	}

	if (!sortedLights.size()) sortedLights.emplace_back(0);
	if (!sortedBlocks.size()) sortedBlocks.emplace_back(0);
	if (!sortedWalls.size()) sortedWalls.emplace_back(0);
	
	pipeline.colorPass->GetBlocksVBO()->Store(sortedBlocks);
	pipeline.colorPass->GetWallsVBO()->Store(sortedWalls);	
	pipeline.lightPass->GetVBO()->Store(sortedLights);

	MW_SYNC_GPU();
}

void MapRenderer::CheckVisibleChunks()
{
	if (Map::VisibleChunks != Map::LastVisibleChunks)
	{
		Map::PopulateVisibleMap();
		Map::LastVisibleChunks = Map::VisibleChunks;
		Map::Flags |= MapFlags_ChunksUpdated;
	}
}

void MapRenderer::PerformRenderPasses(const std::vector<Ref<IRenderer>> &additionalRenderers)
{
	pipeline.colorPass->Perform(camera, sortedWalls.size(), sortedBlocks.size(), additionalRenderers);
	pipeline.lightPass->Perform(camera, sortedLights.size());
}

void MapRenderer::Compose()
{
	pipeline.compositionPass->Perform(pipeline.colorPass, pipeline.lightPass);
}

void MapRenderer::Render(const std::vector<Ref<IRenderer>> &additionalRenderers)
{
	Map::CheckVisibleChunks();

	if (Map::Flags & MapFlags_BlocksUpdated)
	{
		if (Map::Flags & MapFlags_ChunksUpdated)
		{
			UpdateScene();
		}
		PerformRenderPasses(additionalRenderers);
	}
	Compose();

	additionalLightData.clear();

	Map::Flags = MapFlags_None;
}