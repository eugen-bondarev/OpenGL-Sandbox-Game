#pragma once

#include "ColorFBO.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

class ColorPass {
public:
  ColorPass(int amountOfBlocks);
  const Ref<ColorFBO>& GetFBO() const;

  Ref<ColorFBO> fbo;
  Ref<Werwel::Shader> shader;
  Ref<Werwel::VAO> vao;
  Ref<Werwel::VBO> vbo;
  Ref<Werwel::Texture> tileMap;
};