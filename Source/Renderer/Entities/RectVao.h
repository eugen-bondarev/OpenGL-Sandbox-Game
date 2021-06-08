#pragma once

#include "Werwel/VAO.h"
#include "Math/Primitive.h"

#include <memory>

#include <GL/glew.h>

namespace Primitives {

class Rect {
public:
    inline static Ptr<Werwel::VAO> vao;

    inline static void Create() {
        // vao = std::make_unique<VAO>(Primitives::Block::Vertices(1.0f, 1.0f), Vertex::GetLayout(), Primitives::Block::indices);
    }

    inline static void Draw() {
        glDrawElements(GL_TRIANGLES, vao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
    }
};

}