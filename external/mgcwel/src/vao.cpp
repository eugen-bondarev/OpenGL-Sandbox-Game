#include "vao.h"

namespace mw {

VAO::VAO()
{
	glGenVertexArrays(1, &handle);
	MW_DEBUG_LOG_OUT("[Call] Vao constructor");
}

VAO::~VAO()
{
	Unbind();
	glDeleteVertexArrays(1, &handle);

	MW_DEBUG_LOG_OUT("[Call] Vao destructor");
}

}