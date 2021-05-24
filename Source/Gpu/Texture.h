#pragma once

#include "GpuEntity.h"

#include <tuple>

enum class ParamType
{
    Int,
    Float
};

class Texture : public GpuEntity {
private:
    Size m_Size;

    GLint m_InternalFormat;
    GLuint m_Format;
    GLuint m_Type;

public:
    using param_t = std::tuple<ParamType, GLuint, GLfloat>;

    Texture(Size size, unsigned char *data, GLint internalFormat, GLuint format, GLuint type, const std::vector<param_t> parameters);
    ~Texture() override;

    inline Size GetSize() const {
        return m_Size;
    }

    void Resize(Size size);

    void SetParamFV(GLuint name, GLfloat *params);

    void Bind() const override;
    void Bind(GLuint unit) const;
    void Unbind() const override;
};