#include "Texture.h"

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