#include "MapRenderer.h"

#include "Tiles.h"

#include <algorithm>

MapRenderer::MapRenderer(const Ref<Map> &map) : map { map } {

  visibleChunks = map->GetVisibleChunks();
  lastVisibleChunks = visibleChunks;

  pipeline.colorPass = CreateRef<ColorPass>(visibleChunks.GetArea() * map->GetAmountOfChunks().x * map->GetAmountOfChunks().y);
  pipeline.lightPass = CreateRef<LightPass>();
  pipeline.compositionPass = CreateRef<CompositionPass>();

  PopulateBlockData();
}

void MapRenderer::ProcessBlock(BlockData &block, WallData& wallData, Vec2 &light, int x, int y) {
  static int offset = -4;

  if (!map->BlockIsEmpty(x, y)) {
    const Vec2 tile = Vec2(1, 1) + PickRightAngularTile(map->GetBlocks(), x, y);
    block = BlockData(x * map->GetBlockSize(), y * map->GetBlockSize(), tile.x, tile.y);

    if ((map->BlockIsEmpty(x, y - 1) && !map->WallIsEmpty(x, y - 1))
    ||  (map->BlockIsEmpty(x - 1, y) && !map->WallIsEmpty(x - 1, y))
    ||  (map->BlockIsEmpty(x - 1, y - 1) && !map->WallIsEmpty(x - 1, y - 1))
    ) {
      if (!map->WallIs(x, y, WallType::Empty)) {
        const Vec2 tile = Vec2(4, 1) + PickRightAngularTile(map->GetWalls(), x, y);
        wallData = WallData(x * map->GetBlockSize() + offset, (y) * map->GetBlockSize() + offset, tile.x, tile.y);
      }
    }
  } else {
    if (!map->WallIsEmpty(x, y)) {
      const Vec2 tile = Vec2(4, 1) + PickRightAngularTile(map->GetWalls(), x, y);
      wallData = BlockData(x * map->GetBlockSize() + offset, y * map->GetBlockSize() + offset, tile.x, tile.y);
    } else {
      if (!map->BlockIsEmpty(x, y - 1) || !map->WallIsEmpty(x, y - 1)) {
        light = Vec2(16 * x, 16 * y);
      }

      if (!map->BlockIsEmpty(x, y + 1) || !map->WallIsEmpty(x, y - 1)) {
        light = Vec2(16 * x, 16 * y);
      }
    }
  }
}  

void MapRenderer::PopulateBlockData() {
  FORGIO_PROFILER_SCOPE();

  std::vector<BlockData> blocksData;
  std::vector<WallData> wallsData;
  std::vector<Vec2> lightData;

  const auto& chunks = map->GetVisibleChunks();
  const auto& blocks = map->GetBlocks();

  int lights = 0;

  for (int i = chunks.x.start; i < chunks.x.end; i++) {
    for (int j = chunks.y.start; j < chunks.y.end; j++) {
      const bounds_t chunk = {
        { i * static_cast<int>(map->GetChunkSize().x), (i + 1) * static_cast<int>(map->GetChunkSize().x) },
        { j * static_cast<int>(map->GetChunkSize().y), (j + 1) * static_cast<int>(map->GetChunkSize().y) }
      };

      map->GetChunks()[i][j].colorMemOffset = blocksData.size();
      map->GetChunks()[i][j].wallMemOffset = wallsData.size();
      map->GetChunks()[i][j].lightMemOffset = lightData.size();

      for (int x = chunk.x.start; x < chunk.x.end; x++) {
        for (int y = chunk.y.start; y < chunk.y.end; y++) {
          BlockData& newBlock = blocksData.emplace_back();
          WallData& newWall = wallsData.emplace_back();
          Vec2& newLightBlock = lightData.emplace_back(0);
          ProcessBlock(newBlock, newWall, newLightBlock, x, y);
          if (newLightBlock.x != 0 && newLightBlock.y != 0) {
            lights += 1;
          }
        }
      }
    }
  }

  LOG_OUT(lights);

  pipeline.colorPass->GetBlocksVBO()->Update(blocksData, blocksData.size());
  pipeline.colorPass->GetWallsVBO()->Update(wallsData, wallsData.size());
  pipeline.lightPass->GetVBO()->Update(lightData, lightData.size());
}

void MapRenderer::OverrideChunk(MapChunk &oldChunk, MapChunk &newChunk) {
  FORGIO_PROFILER_SCOPE();

  const auto &blocks = map->GetBlocks();

  std::vector<BlockData> newBlocks;
  std::vector<WallData> newWalls;
  std::vector<Vec2> lightBlocks;
  bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
  bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

  for (int x = newChunkBounds.x.start; x < newChunkBounds.x.end; x++) {
    for (int y = newChunkBounds.y.start; y < newChunkBounds.y.end; y++) {
      BlockData &newBlock = newBlocks.emplace_back();
      WallData &newWall = newWalls.emplace_back();
      Vec2 &newLightBlock = lightBlocks.emplace_back(0);
      ProcessBlock(newBlock, newWall, newLightBlock, x, y);
    }
  }

  pipeline.colorPass->GetBlocksVBO()->Update(newBlocks, newBlocks.size(), oldChunk.colorMemOffset);
  pipeline.colorPass->GetWallsVBO()->Update(newWalls, newWalls.size(), oldChunk.wallMemOffset);
  pipeline.lightPass->GetVBO()->Update(lightBlocks, lightBlocks.size(), oldChunk.lightMemOffset);

  newChunk.colorMemOffset = oldChunk.colorMemOffset;
  newChunk.wallMemOffset = oldChunk.wallMemOffset;
  newChunk.lightMemOffset = oldChunk.lightMemOffset;

  rerender = true;

  FORGIO_SYNC_GPU();
}

void MapRenderer::RerenderChunk(int chunkIndexI, int chunkIndexJ) {
  auto& chunk = map->GetChunks()[chunkIndexI][chunkIndexJ];
  OverrideChunk(chunk, chunk);
}

void MapRenderer::Recalculate() {
  FORGIO_PROFILER_SCOPE();

  if (lastVisibleChunks.y.start < visibleChunks.y.start) {
    for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
      OverrideChunk(map->GetChunks()[x][lastVisibleChunks.y.start], map->GetChunks()[x][visibleChunks.y.end - 1]);
    }
  } else if (lastVisibleChunks.y.start > visibleChunks.y.start) {
    for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
      OverrideChunk(map->GetChunks()[x][lastVisibleChunks.y.end - 1], map->GetChunks()[x][visibleChunks.y.start]);
    }
  }

  if (lastVisibleChunks.x.end < visibleChunks.x.end) {
    for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
      OverrideChunk(map->GetChunks()[lastVisibleChunks.x.start][y], map->GetChunks()[visibleChunks.x.end - 1][y]);
    }
  } else if (lastVisibleChunks.x.end > visibleChunks.x.end) {
    for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
      OverrideChunk(map->GetChunks()[lastVisibleChunks.x.end - 1][y], map->GetChunks()[visibleChunks.x.start][y]);
    }
  }
}

void MapRenderer::OnVisibleChunksChange() {
  visibleChunks = map->GetVisibleChunks();

  if (lastVisibleChunks != visibleChunks) {
    Recalculate();
    lastVisibleChunks = visibleChunks;
  }
}

void MapRenderer::Render(Ref<Camera>& camera) {
  FORGIO_PROFILER_SCOPE();

  if (rerender) {
    pipeline.colorPass->Perform(camera, visibleChunks.GetArea() * map->GetChunkSize().x * map->GetChunkSize().y);
    pipeline.lightPass->Perform(camera, visibleChunks.GetArea() * map->GetChunkSize().x * map->GetChunkSize().y);
    rerender = false;
  }

	pipeline.compositionPass->Perform(pipeline.colorPass, pipeline.lightPass);

  FORGIO_SYNC_GPU();
}