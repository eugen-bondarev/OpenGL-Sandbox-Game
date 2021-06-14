#pragma once

#include "ColorFBO.h"

class ColorPass {
public:
  ColorPass();
  const Ref<ColorFBO>& GetFBO() const;

private:
  Ref<ColorFBO> fbo;
};