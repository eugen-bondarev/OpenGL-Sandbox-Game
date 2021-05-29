#pragma once

#include "Math/Primitive.h"

#include "Gpu/Vao.h"
#include "Gpu/Shader.h"

#include "ColorPass.h"

class CompositionPass {
public:
  CompositionPass();
  void Execute(std::shared_ptr<ColorPass>& colorPass);

private:
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Vao> canvas;
};