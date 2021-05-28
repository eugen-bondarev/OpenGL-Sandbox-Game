#pragma once

#include "Texture.h"

#include <GL/glew.h>

#include "GpuEntity.h"

class Framebuffer : public GpuEntity
{
public:
    Framebuffer(Size size, GLbitfield buffersToClear, const std::vector<GLuint>& drawBuffers = {});
    virtual ~Framebuffer();

    void Bind() const override;
    void Unbind() const override;

    void Clear() const;
    virtual void Resize(Size size);

    template <int size>
    std::array<unsigned char, size> GetPixel(GLenum attachment, Pos pos) const
    {
        std::array<unsigned char, size> pixel;

        glReadBuffer(attachment);
        glReadPixels(pos.x, pos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

        return pixel;
    }

    Size GetSize() const;

protected:
    Size m_Size;

    std::map<GLuint, std::shared_ptr<Texture>> m_Attachments;

    void BeginInit();
    void EndInit();

private:
    std::vector<GLuint> m_DrawBuffers;
    GLuint m_BuffersToClear{ GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };
};