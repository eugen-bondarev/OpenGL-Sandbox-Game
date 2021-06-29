#include "LightFBO.h"

LightFBO::LightFBO(Vec2 size) : Werwel::FBO(size, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 }) {
  BeginInit();

  attachments[GL_COLOR_ATTACHMENT0] = CreateRef<Werwel::Texture>(
    size,
    nullptr,
    GL_R8,
    GL_RED,
    GL_UNSIGNED_BYTE,
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_LINEAR },
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_LINEAR }
  );

  EndInit();
}

GLuint LightFBO::GetTextureHandle() const {
  return attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
}

const Ref<Werwel::Texture>& LightFBO::GetTexture() const {
  return attachments.at(GL_COLOR_ATTACHMENT0);
}

void LightFBO::BindTexture(GLuint slot) const {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
}

void LightFBO::UnbindTexture(GLuint slot) const {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}