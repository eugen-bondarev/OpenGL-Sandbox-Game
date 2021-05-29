#pragma once

#include "Framebuffer.h"

class LightFbo : public Framebuffer {
public:
    LightFbo(std::shared_ptr<Texture> & texture);

    void Resize(Size size) override;
    GLuint GetTextureHandle() const;

    void BindTexture(GLuint slot) const;
    void UnbindTexture(GLuint slot) const;
};