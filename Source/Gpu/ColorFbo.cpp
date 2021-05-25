#include "ColorFbo.h"

ColorFbo::ColorFbo(Size size) : Framebuffer(size, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 })
{
    BeginInit();

    m_Attachments[GL_COLOR_ATTACHMENT0] = std::make_unique<Texture>(
        size,
        nullptr,
        GL_RGBA,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        std::vector<Texture::param_t> {
            { ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
            { ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST },
        }
    );

    EndInit();
}

void ColorFbo::Resize(Size size)
{
    Framebuffer::Resize(size);
}

GLuint ColorFbo::GetTextureHandle() const
{
    return m_Attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
}

void ColorFbo::BindTexture(GLuint slot) const {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
}

void ColorFbo::UnbindTexture(GLuint slot) const {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}