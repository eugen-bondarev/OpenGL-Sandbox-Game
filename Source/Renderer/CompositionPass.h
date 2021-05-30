#pragma once

#include "Math/Primitive.h"

#include "Gpu/Vao.h"
#include "Gpu/Shader.h"

#include "ColorPass.h"
#include "LightPass.h"

class CompositionPass {
public:
  CompositionPass();
  void Execute(std::shared_ptr<ColorPass>& colorPass, std::shared_ptr<LightPass>& lightPass);

private:
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Vao> canvas;
};