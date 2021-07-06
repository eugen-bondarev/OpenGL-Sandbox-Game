#include "vbo.h"

namespace mw {

VBO::VBO(GLuint attribute, Type type, Usage usage, size_t amount, size_t size, const void *data, std::vector<VertexBufferLayout> layouts)
{
    this->usage = VBOUsageToGLUsage(usage);
    this->type = VBOTypeToGLUsage(type);
    this->size = size;

    GLuint startAttribute = attribute;
    attributes.resize(layouts.size());

    glGenBuffers(1, &handle);
    glBindBuffer(this->type, handle);

    if (attribute != -1)
    {
        glEnableVertexAttribArray(attribute);
        for (int i = 0; i < attributes.size(); i++)
        {
            int attrib = startAttribute + i;
            glVertexAttribPointer(attrib, layouts[i].size, GL_FLOAT, GL_FALSE, layouts[i].stride, reinterpret_cast<void *>(layouts[i].offset));
            glVertexAttribDivisor(attrib, layouts[i].divisor);

            attributes[i] = attrib;
        }
        glDisableVertexAttribArray(attribute);
    }

    glBufferData(this->type, size * amount, data, this->usage);
    glBindBuffer(this->type, 0);

    if (type == VBO::Type::Indices)
    {
        indexCount = amount;
    }
}

VBO::~VBO()
{
    glBindBuffer(this->type, 0);
    glDeleteBuffers(1, &handle);

    MW_DEBUG_LOG_OUT("[Call] VBO destructor");
}

void VBO::Bind() const
{
    glBindBuffer(this->type, handle);
}

void VBO::Unbind() const
{
    glBindBuffer(this->type, 0);
}

bool VBO::Unmap() const
{
    return glUnmapBuffer(type);
}

GLenum VBO::GetUsage() const
{
    return usage;
}

GLenum VBO::GetType() const
{
    return type;
}

const std::vector<GLuint>& VBO::GetUsedAttributes() const
{
    return attributes;
}

GLuint VBO::GetIndexCount() const
{
    return indexCount;
}

size_t VBO::GetSize() const
{
    return size;
}

GLenum VBO::VBOUsageToGLUsage(VBO::Usage vboUsage)
{
    switch (vboUsage)
    {
    case VBO::Usage::Static:
        return GL_STATIC_DRAW;
    case VBO::Usage::Dynamic:
        return GL_DYNAMIC_DRAW;
    case VBO::Usage::Stream:
        return GL_STREAM_DRAW;
    }

    return GL_STATIC_DRAW;
}

GLenum VBO::VBOTypeToGLUsage(VBO::Type vboUsage)
{
    switch (vboUsage)
    {
    case VBO::Type::Array:
        return GL_ARRAY_BUFFER;
    case VBO::Type::Indices:
        return GL_ELEMENT_ARRAY_BUFFER;
    }

    return GL_ARRAY_BUFFER;
}

}