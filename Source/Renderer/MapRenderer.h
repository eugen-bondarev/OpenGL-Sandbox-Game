#pragma once

#include "Game/Map.h"

#include "ColorPass/ColorPass.h"
#include "LightPass/LightPass.h"
#include "CompositionPass/CompositionPass.h"

class MapRenderer
{
public:
  MapRenderer(const Ref<Map> &map);

  void ProcessBlock(BlockData &block, WallData& wallData, Vec2 &light, int x, int y);
  void PopulateBlockData();
  void OverrideChunk(MapChunk &oldChunk, MapChunk &newChunk);
  void RerenderChunk(int chunkIndexI, int chunkIndexJ);
  void Recalculate();
  void OnVisibleChunksChange();

  void Render(Ref<Camera>& camera);

  inline const bounds_t& GetVisibleChunks() const {
    return visibleChunks;
  }

private:
  struct {
    Ref<ColorPass> colorPass;
    Ref<LightPass> lightPass;
    Ref<CompositionPass> compositionPass;
  } pipeline;

  const Ref<Map>& map;

  bounds_t lastVisibleChunks;
  bounds_t visibleChunks;

  MapRenderer(const MapRenderer &) = delete;
  MapRenderer &operator=(const MapRenderer &) = delete;
};