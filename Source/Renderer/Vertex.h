#pragma once

#include "Math/Math.h"

#include "GPU/VertexBufferLayout.h"

struct Vertex2D {
	Vec2 position;
	Vec2 uv;

	inline static VertexBufferLayouts GetLayout() {
		return {
			{ 2, sizeof(Vertex2D), offsetof(Vertex2D, position) },
			{ 2, sizeof(Vertex2D), offsetof(Vertex2D, uv) }
		};
	}
};