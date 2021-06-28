#include "MapRenderer.h"

#include "Tiles.h"

#include <algorithm>

#include "Util/ImGuiHelper.h"

MapRenderer::MapRenderer(const Ref<Map>& map, const Ref<Camera>& camera) : map { map }, camera { camera } {

  visibleChunks = map->GetVisibleChunks();
  lastVisibleChunks = visibleChunks;

  pipeline.colorPass = CreateRef<ColorPass>(visibleChunks.GetArea() * map->GetAmountOfChunks().x * map->GetAmountOfChunks().y);
  pipeline.lightPass = CreateRef<LightPass>();
  pipeline.compositionPass = CreateRef<CompositionPass>();
}

void MapRenderer::RebuildScene() { 
  NATURAFORGE_PROFILER_SCOPE();

  blocksData.clear();
  wallsData.clear();
  lightData.clear();

  static int offset = -4.0f;

  for (int chunkX = visibleChunks.x.start; chunkX < visibleChunks.x.end; chunkX++) {
    for (int chunkY = visibleChunks.y.start; chunkY < visibleChunks.y.end; chunkY++) {
      for (int x = chunkX * map->GetChunkSize().x; x < (chunkX + 1) * map->GetChunkSize().y; x++) {
        for (int y = chunkY * map->GetChunkSize().y; y < (chunkY + 1) * map->GetChunkSize().y; y++) {
          if (!map->BlockIsEmpty(x, y)) {
            const Vec2 tile = blocksTextureDictionary[map->GetBlocks()[x][y]] + PickRightAngularTile(map->GetBlocks(), x, y);
            blocksData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize(), tile.x, tile.y);

            if (x > 1 && y > 0) {
              if ((map->BlockIsEmpty(x, std::max(y - 1, 0)) && !map->WallIsEmpty(x, y - 1))
              ||  (map->BlockIsEmpty(x - 1, y) && !map->WallIsEmpty(x - 1, y))
              ||  (map->BlockIsEmpty(x - 1, y - 1) && !map->WallIsEmpty(x - 1, y - 1))
              ) {
                if (!map->WallIsEmpty(x, y)) {
                  const Vec2 tile = blocksTextureDictionary[map->GetWalls()[x][y] == WallType::Grass ? WallType::Dirt : map->GetWalls()[x][y]] + PickRightAngularTile(map->GetWalls(), x, y) + Vec2(3, 0);
                  wallsData.emplace_back(x * map->GetBlockSize() + offset, y * map->GetBlockSize() + offset, tile.x, tile.y);
                }
              }
            }
          } else {
            if (!map->WallIsEmpty(x, y)) {
              const Vec2 tile = blocksTextureDictionary[map->GetWalls()[x][y] == WallType::Grass ? WallType::Dirt : map->GetWalls()[x][y]] + PickRightAngularTile(map->GetWalls(), x, y) + Vec2(3, 0);
              wallsData.emplace_back(x * map->GetBlockSize() + offset, y * map->GetBlockSize() + offset, tile.x, tile.y);
            } else {
              if (!map->BlockIsEmpty(x, y - 1) || !map->WallIsEmpty(x, y - 1)) {
                lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
              }

              if (!map->BlockIsEmpty(x, y + 1) || !map->WallIsEmpty(x, y - 1)) {
                lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
              }

              if (!map->BlockIsEmpty(x - 1, y) || !map->WallIsEmpty(x - 1, y)) {
                lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
              }

              if (!map->BlockIsEmpty(x + 1, y) || !map->WallIsEmpty(x + 1, y)) {
                lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
              }
            }
          }
        }
      }
    }
  }
}

void MapRenderer::UpdateScene() {
  NATURAFORGE_PROFILER_SCOPE();
  pipeline.colorPass->GetBlocksVBO()->Store(blocksData);
  pipeline.colorPass->GetWallsVBO()->Store(wallsData);

  std::vector<Vec2> copy = lightData;
  for (const Vec2& light : additionalLightData) {
    copy.push_back(light);
  }
  
  pipeline.lightPass->GetVBO()->Store(copy);
  NATURAFORGE_SYNC_GPU();
}

void MapRenderer::CheckVisibleChunks() {
  visibleChunks = map->GetVisibleChunks();

  if (visibleChunks != lastVisibleChunks) {
    lastVisibleChunks = visibleChunks;
    map->chunksUpdated = true;
  }
}

void MapRenderer::PerformRenderPasses(const std::vector<Ref<IRenderer>>& additionalRenderers) {
  pipeline.colorPass->Perform(camera, wallsData.size(), blocksData.size());
  
  for (const auto& renderer : additionalRenderers) {
    renderer->Render();
  }

  pipeline.lightPass->Perform(camera, lightData.size() + additionalLightData.size());
}

void MapRenderer::Compose() {
  pipeline.compositionPass->Perform(pipeline.colorPass, pipeline.lightPass);
}

void MapRenderer::Render(const std::vector<Ref<IRenderer>>& additionalRenderers) {
  CheckVisibleChunks();

  if (map->blocksUpdated) {
    if (map->chunksUpdated) {
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

const Ref<Werwel::Texture>& MapRenderer::GetTileMap() {
  return pipeline.colorPass->GetTileMap();
}