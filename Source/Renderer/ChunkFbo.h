#pragma once

#include "Gpu/Framebuffer.h"

class ChunkFbo : public Framebuffer
{
public:
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