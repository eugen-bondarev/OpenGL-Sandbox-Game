#include "VAO.h"

VAO::~VAO() {
	Unbind();
	glDeleteBuffers(buffers.size(), buffers.data());
	glDeleteVertexArrays(1, &handle);

	DEBUG_LOG_OUT("[Call] Vao destructor");
}

void VAO::Bind() const {
	glBindVertexArray(handle);

	for (const auto& attribute : attributes) {
		glEnableVertexAttribArray(attribute);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboHandle);
}

void VAO::Unbind() const {
	glBindVertexArray(0);

	for (int i = attributes.size() - 1; i >= 0; --i) {
		glDisableVertexAttribArray(attributes[i]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VAO::Render() const {
	Bind();
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, nullptr);  
	Unbind();
}

GLuint VAO::GetVertexCount() const {
	return vertexCount;
}