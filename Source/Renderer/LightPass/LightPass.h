#pragma once

#include "LightFBO.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Maths/Primitive.h"

#include "Game/Control/Camera.h"

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