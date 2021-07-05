#pragma once

#include "Werwel/werwel.h"

#include "assets/text_asset.h"
#include "../color_pass/color_pass.h"
#include "../light_pass/light_pass.h"

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