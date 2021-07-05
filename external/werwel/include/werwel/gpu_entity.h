#pragma once

#include <GL/glew.h>

namespace ww {

class GPUEntity {
public:
	GPUEntity() = default;
	virtual ~GPUEntity() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	GLuint GetHandle() const;

protected:
	GLuint handle { 0 };

private:
	GPUEntity(const GPUEntity&) = delete;
	GPUEntity& operator=(const GPUEntity&) = delete;
};

}