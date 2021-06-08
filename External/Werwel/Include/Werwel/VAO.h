#pragma once

#include "GpuEntity.h"
#include "VertexBufferLayout.h"
#include "VBO.h"

namespace Werwel {

class VAO : public GpuEntity {
public:
	inline VAO() {
		glGenVertexArrays(1, &handle);
		WERWEL_DEBUG_LOG_OUT("[Call] Vao constructor");
	}

	inline ~VAO() override {		
		Unbind();
		glDeleteVertexArrays(1, &handle);

		WERWEL_DEBUG_LOG_OUT("[Call] Vao destructor");
	}

	inline void Bind() const override {
		glBindVertexArray(handle);

		for (const auto& attribute : attributes) {
			glEnableVertexAttribArray(attribute);
		}
	}

	inline void Unbind() const override {
		for (int i = attributes.size() - 1; i >= 0; --i) {
			glDisableVertexAttribArray(attributes[i]);
		}
		
		glBindVertexArray(0);
	}

	inline GLuint GetVertexCount() const {
		return vertexCount;
	}

	inline void AddVbo(GLuint vbo) {
		buffers.push_back(vbo);
	}

	inline void AddAttribute(GLuint attribute) {
		attributes.push_back(attribute);
	}

	template <typename... Args>
	inline Mem::Ref<VBO> AddVBO(Args... args) {
		Mem::Ref<VBO> vbo = Mem::CreateRef<VBO>(GetLastAttribute(), std::forward<Args>(args)...);

		auto usedAttributes = vbo->GetUsedAttributes();
		for (int i = 0; i < usedAttributes.size(); i++) {
			attributes.push_back(usedAttributes[i]);
		}

		vbos.push_back(vbo);

		if (vbo->GetType() == GL_ELEMENT_ARRAY_BUFFER) {
			indexBuffer = vbo;
			vertexCount = vbo->GetIndexCount();
		}

		return vbo;
	}

	inline GLuint GetLastAttribute() const {
		return static_cast<GLuint>(attributes.size());
	}

	inline const Mem::Ref<VBO>& GetIndexBuffer() const {
		return indexBuffer;
	}

private:
	std::vector<Mem::Ref<VBO>> vbos;
	Mem::Ref<VBO> indexBuffer;

	GLuint vertexCount { 0 };
	
	std::vector<GLuint> attributes;
	std::vector<GLuint> buffers;

	VAO(const VAO &) = delete;
	VAO &operator=(const VAO &) = delete;
};

}