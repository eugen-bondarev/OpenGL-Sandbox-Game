#pragma once

#include "../RenderPass.h"

#include "Resources/ColorFBO.h"

#include "MapRenderer.h"
#include "Game/Map.h"

class ColorPass : public RenderPass<ColorFBO> {
public:
  ColorPass(Ref<Map>& map);
  void Execute(const Mat4& viewMatrix, const Vec2& viewPos);

  inline Ref<MapRenderer>& GetMapRenderer() {
    return mapRenderer;
  }

  inline const light_data_t& GetLightData() const {
    return light;
  }

  struct {
    unsigned int chunksRendered;
  } info;

private:
  light_data_t light;
	Ref<MapRenderer> mapRenderer;
	Ref<Map> map;
	Ref<VAO> chunkVao;
};