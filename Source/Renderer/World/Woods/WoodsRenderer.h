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

  struct {
    Ref<Werwel::Texture> texture;
    Ref<Werwel::Shader> shader;
    Ref<Werwel::VAO> vao;
    Ref<Werwel::VBO> vbo;
  } pipeline;

  WoodsRenderer(const WoodsRenderer&) = delete;
  WoodsRenderer& operator=(const WoodsRenderer&) = delete;
};