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

static std::map<BlockType, Vec2> texts = {
  { BlockType::Dirt, Vec2(1, 1) },
  { BlockType::Grass, Vec2(1, 7) },
};

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
            const Vec2 tile = texts[map->GetBlocks()[x][y]] + PickRightAngularTile(map->GetBlocks(), x, y);
            blocksData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize(), tile.x, tile.y);

            if ((map->BlockIsEmpty(x, y - 1) && !map->WallIsEmpty(x, y - 1))
            ||  (map->BlockIsEmpty(x - 1, y) && !map->WallIsEmpty(x - 1, y))
            ||  (map->BlockIsEmpty(x - 1, y - 1) && !map->WallIsEmpty(x - 1, y - 1))
            ) {
              if (!map->WallIs(x, y, WallType::Empty)) {
                const Vec2 tile = Vec2(1, 1) + PickRightAngularTile(map->GetWalls(), x, y) + Vec2(3, 0);
                wallsData.emplace_back(x * map->GetBlockSize() + offset, (y) * map->GetBlockSize() + offset, tile.x, tile.y);
              }
            }
          } else {
            if (!map->WallIsEmpty(x, y)) {
              const Vec2 tile = Vec2(4, 1) + PickRightAngularTile(map->GetWalls(), x, y);
              wallsData.emplace_back(x * map->GetBlockSize() + offset, y * map->GetBlockSize() + offset, tile.x, tile.y);
            } else {
              if (!map->BlockIsEmpty(x, y - 1) || !map->WallIsEmpty(x, y - 1)) {
                lightData.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize());
              }

              if (!map->BlockIsEmpty(x, y + 1) || !map->WallIsEmpty(x, y - 1)) {
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
  pipeline.colorPass->blocks.vbo->Store(blocksData);
  pipeline.colorPass->walls.vbo->Store(wallsData);

  std::vector<Vec2> copy = lightData;
  for (const Vec2& light : additionalLightData) {
    copy.push_back(light);
  }
  
  pipeline.lightPass->vbo->Store(copy);
  NATURAFORGE_SYNC_GPU();
}

void MapRenderer::CheckVisibleChunks() {
  visibleChunks = map->GetVisibleChunks();

  if (visibleChunks != lastVisibleChunks) {
    lastVisibleChunks = visibleChunks;
    chunksUpdated = true;
  }
}

void MapRenderer::PerformRenderPasses(std::function<void()> AddToFBO) {
  pipeline.colorPass->Perform(camera, wallsData.size(), blocksData.size());
  AddToFBO();
  pipeline.lightPass->Perform(camera, lightData.size() + additionalLightData.size());
}

void MapRenderer::Compose() {
  pipeline.compositionPass->Perform(pipeline.colorPass, pipeline.lightPass);
}

void MapRenderer::Render(std::function<void()> AddToFBO) {
  CheckVisibleChunks();

  if (rerender) {
    if (chunksUpdated) {
      RebuildScene();
      chunksUpdated = false; 
    }
    UpdateScene();    
    PerformRenderPasses(AddToFBO);
  }  
  Compose();

	additionalLightData.clear();

  rerender = false;
}