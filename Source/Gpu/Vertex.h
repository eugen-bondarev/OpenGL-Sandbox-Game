#pragma once

#include "Math/Math.h"

#include "VertexBufferLayout.h"

struct Vertex {
    Vec2 Position;
    Vec2 UV;

    inline static std::vector<VertexBufferLayout> GetLayout() {
        return {
            { 2, sizeof(Vertex), offsetof(Vertex, Position) },
            { 2, sizeof(Vertex), offsetof(Vertex, UV) }
        };
    }
};