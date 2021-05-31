#pragma once

#include "Gpu/Framebuffer.h"

class ColorFbo : public Framebuffer {
public:
    ColorFbo(Size size);

    void Resize(Size size) override;
    GLuint GetTextureHandle() const;

    void BindTexture(GLuint slot = GL_TEXTURE0) const;
    void UnbindTexture(GLuint slot = GL_TEXTURE0) const;
};