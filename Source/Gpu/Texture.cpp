#include "Texture.h"

void Texture::Resize(Size size) {
    m_Size = size;

    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Size.x, m_Size.y, 0, m_Format, m_Type, nullptr);
}

Texture::Texture(Size size, unsigned char *data, GLint internalFormat, GLuint format, GLuint type, const std::vector<param_t> parameters)
    : m_Size{size}, m_InternalFormat{internalFormat}, m_Format{format}, m_Type{type} {
    glGenTextures(1, &m_Handle);
    glBindTexture(GL_TEXTURE_2D, m_Handle);

    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Size.x, m_Size.y, 0, m_Format, m_Type, data);

    for (const auto param : parameters)
    {
        switch (std::get<0>(param))
        {
            case ParamType::Int:
            {
                glTexParameteri(GL_TEXTURE_2D, std::get<1>(param), static_cast<GLint>(std::get<2>(param)));
                break;
            }

            case ParamType::Float:
            {
                glTexParameterf(GL_TEXTURE_2D, std::get<1>(param), std::get<2>(param));
                break;
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &m_Handle);
}

void Texture::SetParamFV(GLuint name, GLfloat *params) {
    glBindTexture(GL_TEXTURE_2D, m_Handle);
    glTexParameterfv(GL_TEXTURE_2D, name, params);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, m_Handle);
}

void Texture::Bind(GLuint unit) const {
    glActiveTexture(unit);
    Bind();
}

void Texture::Unbind(GLuint unit) const {
    glActiveTexture(unit);
    Unbind();
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}