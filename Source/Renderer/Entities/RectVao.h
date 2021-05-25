#pragma once

#include "Gpu/Vao.h"
#include "Math/Primitive.h"

#include <memory>

#include <GL/glew.h>

namespace Primitives {

class Rect {
public:
    inline static std::unique_ptr<Vao> vao;

    inline static void Create() {
        vao = std::make_unique<Vao>(Primitives::Pixel::vertices, Vertex::GetLayout(), Primitives::Pixel::indices);
    }

    inline static void Draw() {
        glDrawElements(GL_TRIANGLES, vao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
    }
};

}