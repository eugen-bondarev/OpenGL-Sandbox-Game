#include "Texture.h"

namespace Werwel {

void Texture::Resize(Size size) {
	size = size;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, nullptr);
}

Texture::~Texture() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &handle);
}

void Texture::SetParamFV(GLuint name, GLfloat *params) {
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameterfv(GL_TEXTURE_2D, name, params);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, handle);
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

void Texture::BindSafely() const {
	if (!IsBound()) {
		Bind();
	}
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