#pragma once

#include "light_fbo.h"

#include "Werwel/texture.h"
#include "Werwel/shader.h"
#include "Werwel/vao.h"

#include "core/window.h"

#include "assets/text_asset.h"
#include "assets/image_asset.h"

#include "maths/primitive.h"

#include "game/entities/camera.h"

class LightPass {
public:  
  LightPass();
  void Perform(const Ref<Camera>& camera, int amountOfBlocks);

  inline const Ref<LightFBO>& GetFBO() const {
    return fbo;
  }

  inline const Ref<Werwel::VBO>& GetVBO() const {
    return vbo;
  }

private:
  Ref<LightFBO> fbo;
  Ref<Werwel::Shader> shader;
  Ref<Werwel::VBO> vbo;

  struct {
    Ref<Werwel::VAO> vao;
    Ref<Werwel::Texture> texture;
  } lightMesh;
  std::vector<Vec2> lightPositions;
};