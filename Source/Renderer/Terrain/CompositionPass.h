#pragma once

#include "Maths/Primitive.h"

#include "Werwel/VAO.h"
#include "Werwel/Shader.h"

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