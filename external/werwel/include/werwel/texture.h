#pragma once

#include "gpu_entity.h"

namespace ww {

enum class Interpolation
{
	Linear,
	Constant
};

class Texture : public GPUEntity 
{
public:
	using Pixels_t = const unsigned char*;

	using Parameter_t = std::function<void()>;
	using Parameters_t = std::vector<Parameter_t>;

	Texture(Vec2 size, Pixels_t data, GLint internalFormat, GLuint format, GLuint type, Parameters_t params = {});
	~Texture() override;

	static Parameter_t SetInterpolation(Interpolation interpolation);

	void Resize(Vec2 size);

	void Bind() const override;
	void Bind(GLuint unit) const;
	void Unbind() const override;
	void Unbind(GLuint unit) const;

	Vec2 GetSize() const;
	
protected:
	Vec2 size;

	GLint internalFormat;
	GLuint format;
	GLuint type;

private:
	Texture(const Texture&) = delete;
	Texture operator=(const Texture&) = delete;
};

}