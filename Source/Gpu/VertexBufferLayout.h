#pragma once

#include <vector>

struct VertexBufferLayout {
	size_t Size   { 0 };
	size_t Stride { 0 };
	size_t Offset { 0 };
};

using VertexBufferLayouts = std::vector<VertexBufferLayout>;