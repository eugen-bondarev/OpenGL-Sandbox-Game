#pragma once

#include <vector>

namespace mw {

struct VertexBufferLayout 
{
	size_t size   { 0 };
	size_t stride { 0 };
	size_t offset { 0 };
	int divisor 	{ 0 };
};

using VertexBufferLayouts = std::vector<VertexBufferLayout>;

}