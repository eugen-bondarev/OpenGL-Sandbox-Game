#include "VAO.h"

VAO::~VAO() {
	Unbind();
	glDeleteBuffers(buffers.size(), buffers.data());
	glDeleteVertexArrays(1, &handle);

	DEBUG_LOG_OUT("[Call] Vao destructor");
}