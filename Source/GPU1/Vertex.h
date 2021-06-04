#pragma once

#include "Math/Math.h"

#include "VertexBufferLayout.h"

struct Vertex {
	Vec2 position;
	Vec2 uv;

	inline static VertexBufferLayouts GetLayout() {
		return {
			{ 2, sizeof(Vertex), offsetof(Vertex, position) },
			{ 2, sizeof(Vertex), offsetof(Vertex, uv) }
		};
	}
};