#pragma once

#include "Framebuffer.h"

class ColorFbo : public Framebuffer {
public:
    ColorFbo(Size size);

    void Resize(Size size) override;
    GLuint GetTextureHandle() const;
};