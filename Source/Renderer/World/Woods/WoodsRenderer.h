#pragma once

#include "Game/World/Woods/Woods.h"
#include "Game/Entities/Camera.h"
#include "Renderer/Renderer.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

class WoodsRenderer : public IRenderer {
public:
  WoodsRenderer(const Ref<Woods>& woods, const Ref<Camera>& camera);

  void Render() override;

  std::vector<Vec2> visibleTrees;

private:
  const Ref<Woods>& woods;
  const Ref<Camera>& camera;

  Vec2 barkSize { 16, 204 };
  Vec2 leavesSize { 128, 128 };

  Mat4 barkModelMatrix { 1 };
  Mat4 leavesModelMatrix { 1 };

  struct {
    Ref<Werwel::Texture> barkTexture;
    Ref<Werwel::Texture> leavesTexture;
    
    Ref<Werwel::Shader> shader;

    Ref<Werwel::VAO> barkVAO;
    Ref<Werwel::VBO> vbo;
  } pipeline;

  WoodsRenderer(const WoodsRenderer&) = delete;
  WoodsRenderer& operator=(const WoodsRenderer&) = delete;
};