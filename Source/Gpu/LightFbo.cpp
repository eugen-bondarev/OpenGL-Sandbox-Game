#include "LightFbo.h"

LightFbo::LightFbo(Size size) : Framebuffer(size, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 })
{
    BeginInit();

    m_Attachments[GL_COLOR_ATTACHMENT0] = std::make_shared<Texture>(
        size,
        nullptr,
        /*GL_R8*/ GL_RGBA,
        /*GL_RED*/GL_RGBA,
        GL_UNSIGNED_BYTE,
        std::vector<Texture::param_t> {
            { ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
            { ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST },
        }
    );

    EndInit();
}

void LightFbo::Resize(Size size)
{
    Framebuffer::Resize(size);
}

GLuint LightFbo::GetTextureHandle() const
{
    return m_Attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
}

void LightFbo::BindTexture(GLuint slot) const {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
}

void LightFbo::UnbindTexture(GLuint slot) const {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}