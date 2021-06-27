#pragma once

#include "Game/Map.h"

#include "ColorPass/ColorPass.h"
#include "LightPass/LightPass.h"
#include "CompositionPass/CompositionPass.h"

#include "Renderer.h"

inline static std::map<BlockType, Vec2> blocksTextureDictionary = {
  { BlockType::Dirt, Vec2(1, 1) },
  { BlockType::Grass, Vec2(1, 7) },
  { BlockType::Stone, Vec2(7, 1) },
  { BlockType::Wood, Vec2(13, 1) },
};

class MapRenderer
{
public:
  MapRenderer(const Ref<Map> &map, const Ref<Camera>& camera);

  void RebuildScene();
  void UpdateScene();
  void CheckVisibleChunks();
  void PerformRenderPasses(const std::vector<Ref<IRenderer>>& additionalRenderers);
  void Compose();

  void Render(const std::vector<Ref<IRenderer>>& additionalRenderers);

  inline const bounds_t& GetVisibleChunks() const {
    return visibleChunks;
  }
  
  inline int GetAmountOfRenderedBlocks() const {
    return blocksData.size();
  }
  
  inline int GetAmountOfRenderedWalls() const {
    return wallsData.size();
  }
  
  inline int GetAmountOfRenderedLights() const {
    return lightData.size();
  }

  inline std::vector<Vec2>& GetAdditionalLightData() {
    return additionalLightData;
  }

  const Ref<Werwel::Texture>& GetTileMap();
  
private:
  struct {
    Ref<ColorPass> colorPass;
    Ref<LightPass> lightPass;
    Ref<CompositionPass> compositionPass;
  } pipeline;

  std::vector<Vec2> additionalLightData;

  std::vector<BlockData> blocksData;
  std::vector<WallData> wallsData;
  std::vector<Vec2> lightData;

  const Ref<Map>& map;
  const Ref<Camera>& camera;

  bounds_t lastVisibleChunks;
  bounds_t visibleChunks;

  MapRenderer(const MapRenderer &) = delete;
  MapRenderer &operator=(const MapRenderer &) = delete;
};