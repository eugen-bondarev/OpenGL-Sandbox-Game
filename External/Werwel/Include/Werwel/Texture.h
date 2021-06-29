#pragma once

#include "GpuEntity.h"

namespace Werwel {

class Texture : public GpuEntity {
public:
	enum class ParamType {
		Int = 0,
		Float
	};

	using param_t = std::tuple<ParamType, GLuint, GLfloat>;
	using pixels_t = const unsigned char*;

	template <typename... Args>
	Texture(Size size, pixels_t data, GLint internalFormat, GLuint format, GLuint type, Args... parameters) 
		: size { size }, internalFormat { internalFormat }, format { format }, type { type } {		
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

	void BindSafely() const;

	inline static Texture const* boundTexture { nullptr };
	inline bool IsBound() const {
		return this == boundTexture;
	}
	
protected:
	Size size;

	GLint internalFormat;
	GLuint format;
	GLuint type;

private:
	Texture(const Texture&) = delete;
	Texture operator=(const Texture&) = delete;
};

}