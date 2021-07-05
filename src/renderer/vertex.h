#pragma once

#include "maths/maths.h"

#include "Werwel/werwel.h"

struct Vertex2D {
	Vec2 position;
	Vec2 uv;

	inline static Werwel::VertexBufferLayouts GetLayout() {
		return {
			{ 2, sizeof(Vertex2D), offsetof(Vertex2D, position) },
			{ 2, sizeof(Vertex2D), offsetof(Vertex2D, uv) }
		};
	}
};