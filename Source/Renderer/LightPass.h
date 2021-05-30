#pragma once

#include "RenderPass.h"

#include "Gpu/LightFbo.h"
#include "Gpu/Texture.h"

#include "MapRenderer.h"
#include "ColorPass.h"

#include "Game/Map.h"

class LightPass : public RenderPass<LightFbo> {
public:
  LightPass(std::shared_ptr<MapRenderer>& mapRenderer);
  void Execute(std::shared_ptr<ColorPass>& colorPass, std::shared_ptr<Map>& map, const Mat4& viewMatrix, const Vec2& viewPos);

private:
  GLuint transformationVbo;
  std::shared_ptr<MapRenderer> mapRenderer;
  std::shared_ptr<Vao> lightVao;
  std::shared_ptr<Texture> lightTexture;
};