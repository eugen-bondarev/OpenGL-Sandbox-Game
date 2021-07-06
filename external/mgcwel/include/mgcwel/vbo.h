#pragma once

#include "gpu_entity.h"

#include "vertex_buffer_layout.h"

namespace mw {

class VBO : public GPUEntity
{
public:
	enum class Usage
	{
		Static,
		Dynamic,
		Stream
	};

	enum class Type
	{
		Array,
		Indices
	};

	VBO(GLuint attribute, Type type, Usage usage, size_t amount = 0, size_t size = 0, const void *data = nullptr, std::vector<VertexBufferLayout> layouts = {});
	~VBO() override;

	void Bind() const override;
	void Unbind() const override;

	template <bool bind = true>
	void Store(size_t amount, const void *data) const
	{
		if (bind)
		{
			glBindBuffer(type, handle);
		}
		glBufferData(type, size * amount, data, usage);
	}

	template <typename T, bool bind = true>
	void Store(const std::vector<T> &vec) const
	{
		if (bind)
		{
			glBindBuffer(type, handle);
		}
		glBufferData(type, vec.size() * size, vec.data(), usage);
	}

	template <typename T, bool bind = true>
	void Update(const std::vector<T> &vec, int amount, int pos = 0) const
	{
		if (bind)
		{
			glBindBuffer(type, handle);
		}
		glBufferSubData(type, size * pos, size * amount, vec.data());
	}

	template <bool bind = true>
	void *Map() const
	{
		if (bind)
		{
			glBindBuffer(type, handle);
		}
		return glMapBuffer(type, GL_WRITE_ONLY);
	}

	bool Unmap() const;
	GLenum GetUsage() const;
	GLenum GetType() const;
	const std::vector<GLuint> &GetUsedAttributes() const;
	GLuint GetIndexCount() const;
	size_t GetSize() const;

private:
	std::vector<GLuint> attributes;

	GLuint indexCount;
	GLenum type;
	GLenum usage;
	size_t size;

	static GLenum VBOUsageToGLUsage(VBO::Usage vboUsage);
	static GLenum VBOTypeToGLUsage(VBO::Type vboUsage);

	VBO(const VBO &) = delete;
	VBO &operator=(const VBO &) = delete;
};

}