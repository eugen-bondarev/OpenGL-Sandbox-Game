#include "Framebuffer.h"

Framebuffer::Framebuffer(Size size, GLbitfield buffersToClear, const std::vector<GLuint>& drawBuffers)
    : m_Size{ size }, m_BuffersToClear{ buffersToClear }, m_DrawBuffers{ drawBuffers }
{
}

Framebuffer::~Framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &m_Handle);
}

void Framebuffer::BeginInit()
{
    glGenFramebuffers(1, &m_Handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
}

void Framebuffer::EndInit()
{
    for (auto& attachment : m_Attachments)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.first, GL_TEXTURE_2D, attachment.second->GetHandle(), 0);
    }

    if (m_DrawBuffers.size() > 0)
    {
        glDrawBuffers(m_DrawBuffers.size(), m_DrawBuffers.data());
    }
    else
    {
        glDrawBuffer(GL_NONE);
    }

    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer is not complete.\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
}

void Framebuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Clear() const
{
    glClear(m_BuffersToClear);
}

void Framebuffer::Resize(Size size)
{
    m_Size = size;

    for (auto& attachment : m_Attachments)
    {
        attachment.second->Bind();
        attachment.second->Resize(size);
        attachment.second->Unbind();
    }
}

Size Framebuffer::GetSize() const
{
    return m_Size;
}