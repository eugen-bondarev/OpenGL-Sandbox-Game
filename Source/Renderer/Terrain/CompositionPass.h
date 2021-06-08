#pragma once

#include "Math/Primitive.h"

#include "GPU/VAO.h"
#include "GPU/Shader.h"

#include "ColorPass.h"
#include "LightPass.h"

class CompositionPass {
public:
  CompositionPass();
  void Execute(Ref<ColorPass>& colorPass, Ref<LightPass>& lightPass);

private:
  Ref<Werwel::Shader> shader;
  Ref<Werwel::VAO> canvas;
};