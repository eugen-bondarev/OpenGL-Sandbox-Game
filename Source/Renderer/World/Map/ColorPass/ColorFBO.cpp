#include "ColorFBO.h"

ColorFBO::ColorFBO(Vec2 size) : Werwel::FBO(size, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 }) {
  BeginInit();

  attachments[GL_COLOR_ATTACHMENT0] = CreateRef<Werwel::Texture>(
    size,
    nullptr,
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
  );

  EndInit();
}

GLuint ColorFBO::GetTextureHandle() const {
  return attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
}

const Ref<Werwel::Texture>& ColorFBO::GetTexture() const {
  return attachments.at(GL_COLOR_ATTACHMENT0);
}

void ColorFBO::BindTexture(GLuint slot) const {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
}

void ColorFBO::UnbindTexture(GLuint slot) const {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}