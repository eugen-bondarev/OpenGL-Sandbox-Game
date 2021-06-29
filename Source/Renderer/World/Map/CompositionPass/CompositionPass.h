#pragma once

#include "Werwel/Shader.h"
#include "Werwel/VAO.h"
#include "Werwel/GraphicsContext.h"

#include "Assets/TextAsset.h"
#include "../ColorPass/ColorPass.h"
#include "../LightPass/LightPass.h"

class CompositionPass {
public:
  CompositionPass();
  void Perform(const Ref<ColorPass>& colorPass, const Ref<LightPass>& lightPass);

  Ref<Werwel::Shader> shader;
  Ref<Werwel::VAO> canvas;
  
private:
  struct {
    Ref<Werwel::Shader> shader;
  } sky;
};