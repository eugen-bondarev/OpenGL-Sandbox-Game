#include "Texture.h"

namespace Werwel {

Texture::Texture(Size size, Pixels_t data, GLint internalFormat, GLuint format, GLuint type, Parameters_t params) : size{size}, internalFormat{internalFormat}, format{format}, type{type}
{
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);

	for (int i = 0; i < params.size(); i++)
	{
		params[i]();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

Size Texture::GetSize() const
{
	return size;
}

void Texture::Resize(Size size)
{
	this->size = size;
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, nullptr);
}

Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &handle);
}

void Texture::SetParamFV(GLuint name, GLfloat *params)
{
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameterfv(GL_TEXTURE_2D, name, params);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::Bind(GLuint unit) const
{
	glActiveTexture(unit);
	Bind();
}

void Texture::Unbind(GLuint unit) const
{
	glActiveTexture(unit);
	Unbind();
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void LinearInterpolation()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ConstantInterpolation()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::Parameter_t Texture::SetInterpolation(Interpolation interpolation)
{
	return interpolation == Interpolation::Linear ? LinearInterpolation : ConstantInterpolation;
}

}