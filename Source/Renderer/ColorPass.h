#pragma once

#include "RenderPass.h"

#include "Gpu/ColorFbo.h"

#include "MapRenderer.h"
#include "Game/Map.h"

class ColorPass : public RenderPass<ColorFbo> {
public:
  ColorPass(std::shared_ptr<Map>);
  void Execute(const Mat4& viewMatrix, const Vec2& viewPos);

  inline std::shared_ptr<MapRenderer>& GetMapRenderer() {
    return mapRenderer;
  }

  struct {
    unsigned int chunksRendered;
  } info;

  std::vector<Pos> light;

private:
	static bounds_t GetVisibleChunks(std::shared_ptr<Map>& map, Pos viewPos);
	std::shared_ptr<MapRenderer> mapRenderer;
	std::shared_ptr<Map> map;
	std::shared_ptr<Vao> chunkVao;
};