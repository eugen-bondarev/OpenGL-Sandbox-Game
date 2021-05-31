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

  struct {
    unsigned int chunksRendered;
  } info;

  std::vector<Pos> light;

private:
	Ref<MapRenderer> mapRenderer;
	Ref<Map> map;
	Ref<VAO> chunkVao;
};