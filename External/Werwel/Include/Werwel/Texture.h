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

	Texture(Size size, Pixels_t data, GLint internalFormat, GLuint format, GLuint type, Parameters_t params = {});
	~Texture() override;

	Size GetSize() const;

	void Resize(Size size);

	void SetParamFV(GLuint name, GLfloat *params);

	void Bind() const override;
	void Bind(GLuint unit) const;
	void Unbind() const override;
	void Unbind(GLuint unit) const;
	
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