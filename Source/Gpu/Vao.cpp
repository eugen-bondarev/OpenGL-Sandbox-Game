#include "Vao.h"

Vao::~Vao()
{
    Unbind();
    glDeleteBuffers(m_Buffers.size(), m_Buffers.data());
    glDeleteVertexArrays(1, &m_Handle);

    DEBUG_LOG_OUT("[Call] Vao destructor");
}

void Vao::Bind() const
{
    glBindVertexArray(m_Handle);

    for (const auto& attribute : m_Attributes)
    {
        glEnableVertexAttribArray(attribute);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboHandle);
}

void Vao::Unbind() const
{
    glBindVertexArray(0);

    for (int i = m_Attributes.size() - 1; i >= 0; --i)
    {
        glDisableVertexAttribArray(m_Attributes[i]);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint Vao::GetVertexCount() const
{
    return m_VertexCount;
}