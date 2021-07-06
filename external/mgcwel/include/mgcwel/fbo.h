#pragma once

#include "texture.h"

#include <GL/glew.h>

#include "gpu_entity.h"

#include "pch.h"

namespace mw {

class FBO : public GPUEntity
{
public:
	FBO(Vec2 size, GLbitfield buffersToClear, std::vector<GLuint> drawBuffers = {});
	virtual ~FBO();

	void Bind() const override;
	void Unbind() const override;

	void Clear() const;
	virtual void Resize(Vec2 size);

	template <int size>
	std::array<unsigned char, size> GetPixel(GLenum attachment, Vec2 pos) const
	{
		std::array<unsigned char, size> pixel;

		glReadBuffer(attachment);
		glReadPixels(pos.x, pos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

		return pixel;
	}

	Vec2 GetSize() const;

protected:
	Vec2 size;

	std::map<GLuint, std::shared_ptr<Texture>> attachments;

	void BeginInit();
	void EndInit();

private:
	std::vector<GLuint> drawBuffers;
	GLuint buffersToClear { GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT };

	FBO(const FBO &) = delete;
	FBO operator=(const FBO &) = delete;
};

}