#pragma once

#include "GpuEntity.h"

#include <tuple>

enum class ParamType {
	Int = 0,
	Float
};

class Texture : public GpuEntity {
public:
	using param_t = std::tuple<ParamType, GLuint, GLfloat>;

	Texture(Size size, const unsigned char *data, GLint internalFormat, GLuint format, GLuint type, const std::vector<param_t> parameters);
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