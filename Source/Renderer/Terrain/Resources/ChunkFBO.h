#pragma once

#include "GPU/FBO.h"

class ChunkFBO : public FBO {
public:
  ChunkFBO(std::shared_ptr<Texture> & texture) : FBO(texture->GetSize(), GL_COLOR_BUFFER_BIT, {GL_COLOR_ATTACHMENT0}) {
    BeginInit();

    attachments[GL_COLOR_ATTACHMENT0] = texture;

    EndInit();
  }

  void Resize(Size size) {
    FBO::Resize(size);
  }

  GLuint GetTextureHandle() const {
    return attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
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