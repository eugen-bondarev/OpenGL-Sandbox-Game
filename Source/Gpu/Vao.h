#pragma once

#include "GpuEntity.h"

#include "VertexBufferLayout.h"

class VAO : public GpuEntity {
public:
	template <typename T_Vertex>
	VAO(const std::vector<T_Vertex> &vertices, const std::vector<VertexBufferLayout> &layouts, const std::vector<int> &indices) {
		attributes.resize(layouts.size());

		glGenVertexArrays(1, &handle);
		glBindVertexArray(handle);

			GLuint vbo{ 0 };
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T_Vertex), &vertices[0], GL_STATIC_DRAW);       
				for (int i = 0; i < layouts.size(); i++) {
					glVertexAttribPointer(i, layouts[i].Size, GL_FLOAT, GL_FALSE, layouts[i].Stride, reinterpret_cast<void *>(layouts[i].Offset));
					attributes[i] = i;
				}
				buffers.emplace_back(vbo);

			glGenBuffers(1, &indexVboHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboHandle);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				buffers.emplace_back(indexVboHandle);

			vertexCount = static_cast<GLuint>(indices.size());

		glBindVertexArray(0);

		DEBUG_LOG_OUT("[Call] Vao constructor");
	}
	~VAO() override;

	void Bind() const override;
	void Unbind() const override;

	void Render() const;

	GLuint GetVertexCount() const;

	inline void AddVbo(GLuint vbo) {
		buffers.push_back(vbo);
	}

	inline void AddAttribute(GLuint attribute) {
		attributes.push_back(attribute);
	}

	inline GLuint GetLastAttribute() const {
		return static_cast<GLuint>(attributes.size());
	}

private:
	GLuint indexVboHandle{0};
	GLuint vertexCount{0};
	std::vector<GLuint> attributes;
	std::vector<GLuint> buffers;

	VAO(const VAO &) = delete;
	VAO &operator=(const VAO &) = delete;
};