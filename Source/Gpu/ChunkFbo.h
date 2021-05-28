#pragma once

#include "Framebuffer.h"

class ChunkFbo : public Framebuffer
{
public:
  ChunkFbo(Size size) : Framebuffer(size, GL_COLOR_BUFFER_BIT, {GL_COLOR_ATTACHMENT0}) {
    BeginInit();

    m_Attachments[GL_COLOR_ATTACHMENT0] = std::make_unique<Texture>(
      size,
      nullptr,
      GL_RGBA,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      std::vector<Texture::param_t>{
        {ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST},
        {ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST},
    });

    EndInit();
  }

  ChunkFbo(std::shared_ptr<Texture> & texture) : Framebuffer(texture->GetSize(), GL_COLOR_BUFFER_BIT, {GL_COLOR_ATTACHMENT0}) {
    BeginInit();

    m_Attachments[GL_COLOR_ATTACHMENT0] = texture;

    EndInit();
  }

  void Resize(Size size) {
    Framebuffer::Resize(size);
  }

  GLuint GetTextureHandle() const {
    return m_Attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
  }

  void BindTexture(GLuint slot) const {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
  }

  void UnbindTexture(GLuint slot) const {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
};