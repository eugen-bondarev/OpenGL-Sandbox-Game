#include "ColorPass.h"

#include "Core/Window.h"

ColorPass::ColorPass() {
  fbo = CreateRef<ColorFBO>(Window::GetSize());
}

const Ref<ColorFBO>& ColorPass::GetFBO() const {
  return fbo;
}