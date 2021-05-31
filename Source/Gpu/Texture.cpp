#include "Texture.h"

void Texture::Resize(Size size) {
	size = size;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, nullptr);
}

Texture::Texture(Size size, const unsigned char *data, GLint internalFormat, GLuint format, GLuint type, const std::vector<param_t> parameters)
	: size{size}, internalFormat{internalFormat}, format{format}, type{type} {
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);

	for (const auto param : parameters) {
		switch (std::get<0>(param)) {
			case ParamType::Int: {
				glTexParameteri(GL_TEXTURE_2D, std::get<1>(param), static_cast<GLint>(std::get<2>(param)));
				break;
			}

			case ParamType::Float: {
				glTexParameterf(GL_TEXTURE_2D, std::get<1>(param), std::get<2>(param));
				break;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
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