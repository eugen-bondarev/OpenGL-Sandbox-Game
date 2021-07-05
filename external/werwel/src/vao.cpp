#include "vao.h"

namespace ww {

VAO::VAO()
{
	glGenVertexArrays(1, &handle);
	WERWEL_DEBUG_LOG_OUT("[Call] Vao constructor");
}

VAO::~VAO()
{
	Unbind();
	glDeleteVertexArrays(1, &handle);

	WERWEL_DEBUG_LOG_OUT("[Call] Vao destructor");
}

}