#pragma once

#include <GL/glew.h>

#include "Common.h"

namespace Werwel {

class GpuEntity {
public:
	GpuEntity() = default;
	virtual ~GpuEntity() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	inline GLuint GetHandle() const {
		return handle; 
	}

	inline bool IsBound() const {
		return isBound;
	}

protected:
	GLuint handle { 0 };

	bool isBound;

private:
	GpuEntity(const GpuEntity&) = delete;
	GpuEntity& operator=(const GpuEntity&) = delete;
};

}