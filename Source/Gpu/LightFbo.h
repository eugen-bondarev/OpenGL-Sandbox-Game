#pragma once

#include "Framebuffer.h"

class LightFbo : public Framebuffer {
public:
    LightFbo(Size size);

    void Resize(Size size) override;
    GLuint GetTextureHandle() const;

    void BindTexture(GLuint slot) const;
    void UnbindTexture(GLuint slot) const;
};