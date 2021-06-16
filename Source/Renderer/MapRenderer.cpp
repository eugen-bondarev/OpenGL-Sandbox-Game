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

void MapRenderer::ProcessBlock(Vec4 &block, Vec2 &light, int x, int y) {
  if (map->GetBlocks()[x][y] != BlockType::Empty) {
    const Vec2 tile = Vec2(1, 1) + PickRightAngularTile(map->GetBlocks(), x, y);
    block = Vec4(x * map->GetBlockSize(), y * map->GetBlockSize(), tile.x, tile.y);
  } else {
    if (map->GetBlocks()[x][y - 1] != BlockType::Empty) {
      light = Vec2(16 * x, 16 * y);
    }
    if (map->GetBlocks()[x][y + 1] != BlockType::Empty) {
      light = Vec2(16 * x, 16 * y);
    }
  }
}  

void MapRenderer::PopulateBlockData() {
  FORGIO_PROFILER_SCOPE();

  std::vector<Vec4> blocksData;
  std::vector<Vec2> lightData;

  const auto& chunks = map->GetVisibleChunks();
  const auto& blocks = map->GetBlocks();

  for (int i = chunks.x.start; i < chunks.x.end; i++) {
    for (int j = chunks.y.start; j < chunks.y.end; j++) {
      const bounds_t chunk = {
        { i * static_cast<int>(map->GetChunkSize().x), (i + 1) * static_cast<int>(map->GetChunkSize().x) },
        { j * static_cast<int>(map->GetChunkSize().y), (j + 1) * static_cast<int>(map->GetChunkSize().y) }
      };

      map->GetChunks()[i][j].colorMemOffset = blocksData.size();
      map->GetChunks()[i][j].lightMemOffset = lightData.size();

      for (int x = chunk.x.start; x < chunk.x.end; x++) {
        for (int y = chunk.y.start; y < chunk.y.end; y++) {
          Vec4& newBlock = blocksData.emplace_back();
          Vec2& newLightBlock = lightData.emplace_back();
          ProcessBlock(newBlock, newLightBlock, x, y);
        }
      }
    }
  }

  // std::sort(blocksData.begin(), blocksData.end(), [](const BlockData& a, const BlockData& b) -> bool {
  //   return a.tile.x > b.tile.x;
  // });

  pipeline.colorPass->GetVBO()->Update(blocksData, blocksData.size());
  pipeline.lightPass->GetVBO()->Update(lightData, lightData.size());
}

void MapRenderer::OverrideChunk(MapChunk &oldChunk, MapChunk &newChunk) {
  FORGIO_PROFILER_SCOPE();

  const auto &blocks = map->GetBlocks();

  std::vector<Vec4> newBlocks;
  std::vector<Vec2> lightBlocks;
  bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
  bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

  for (int x = newChunkBounds.x.start; x < newChunkBounds.x.end; x++) {
    for (int y = newChunkBounds.y.start; y < newChunkBounds.y.end; y++) {
      Vec4 &newBlock = newBlocks.emplace_back(0);
      Vec2 &newLightBlock = lightBlocks.emplace_back(0);
      ProcessBlock(newBlock, newLightBlock, x, y);
    }
  }

  pipeline.colorPass->GetVBO()->Update(newBlocks, newBlocks.size(), oldChunk.colorMemOffset);
  pipeline.lightPass->GetVBO()->Update(lightBlocks, lightBlocks.size(), oldChunk.lightMemOffset);

  newChunk.colorMemOffset = oldChunk.colorMemOffset;
  newChunk.lightMemOffset = oldChunk.lightMemOffset;

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
	pipeline.colorPass->Perform(camera, visibleChunks.GetArea() * map->GetChunkSize().x * map->GetChunkSize().y);
	pipeline.lightPass->Perform(camera, visibleChunks.GetArea() * map->GetChunkSize().x * map->GetChunkSize().y);
	pipeline.compositionPass->Perform(pipeline.colorPass, pipeline.lightPass);
}