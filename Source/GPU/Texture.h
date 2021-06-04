#pragma once

#include "GpuEntity.h"

#include <tuple>

class Texture : public GpuEntity {
public:
	enum class ParamType {
		Int = 0,
		Float
	};

	using param_t = std::tuple<ParamType, GLuint, GLfloat>;

	template <typename... Args>
	Texture(
		Size size, 
		const unsigned char *data, 
		GLint internalFormat, 
		GLuint format, 
		GLuint type, 
		Args... parameters
	) : size { size }, 
	internalFormat { internalFormat }, 
	format { format }, 
	type { type } {		
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);

		std::vector<param_t> params { parameters... };

		for (const auto param : params) {
			switch (std::get<0>(param)) {
				case Texture::ParamType::Int: {
					glTexParameteri(GL_TEXTURE_2D, std::get<1>(param), static_cast<GLint>(std::get<2>(param)));
					break;
				}

				case Texture::ParamType::Float: {
					glTexParameterf(GL_TEXTURE_2D, std::get<1>(param), std::get<2>(param));
					break;
				}
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~Texture() override;

	inline Size GetSize() const {
		return size;
	}

	void Resize(Size size);

	void SetParamFV(GLuint name, GLfloat *params);

	void Bind() const override;
	void Bind(GLuint unit) const;
	void Unbind() const override;
	void Unbind(GLuint unit) const;
	
private:
	Size size;

	GLint internalFormat;
	GLuint format;
	GLuint type;

	Texture(const Texture&) = delete;
	Texture operator=(const Texture&) = delete;
};