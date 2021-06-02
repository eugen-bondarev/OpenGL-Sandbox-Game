#pragma once

#include "../RenderPass.h"

#include "GPU/Texture.h"

#include "MapRenderer.h"
#include "ColorPass.h"
#include "Resources/LightFBO.h"

#include "Game/Map.h"

class LightPass : public RenderPass<LightFBO> {
public:
  LightPass(Ref<MapRenderer>& mapRenderer);
  void Execute(Ref<ColorPass>& colorPass, Ref<Map>& map, const Mat4& viewMatrix, const Vec2& viewPos, bool chunksChanged);

private:
  Ref<VBO> transformationVBO;
  Ref<Texture> lightTexture;
  Ref<MapRenderer> mapRenderer;
  Ref<VAO> lightVao;
};