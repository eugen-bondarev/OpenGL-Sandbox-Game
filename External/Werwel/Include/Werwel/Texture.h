#pragma once

#include "GpuEntity.h"

#include <functional>

namespace Werwel {

enum class Interpolation
{
	Linear,
	Constant
};

class Texture : public GpuEntity {
public:
	using Pixels_t = const unsigned char*;

	using Parameter_t = std::function<void()>;
	using Parameters_t = std::vector<Parameter_t>;

	static Parameter_t SetInterpolation(Interpolation interpolation);

	Texture(Size size, Pixels_t data, GLint internalFormat, GLuint format, GLuint type, Parameters_t params = {}) 
		: size { size }, internalFormat { internalFormat }, format { format }, type { type } 
	{		
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, data);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		for (int i = 0; i < params.size(); i++) {
			params[i]();
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