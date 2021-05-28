#pragma once

#include "Gpu/Vertex.h"

#include <vector>
    
namespace Primitives {
namespace Quad {

inline static int tileSize = 16;

inline static std::vector<Vertex> vertices = {
    { { -tileSize / 2, -tileSize / 2 }, { 0, 1 } },
    { {  tileSize / 2, -tileSize / 2 }, { 1, 1 } },
    { { -tileSize / 2,  tileSize / 2 }, { 0, 0 } },
    { {  tileSize / 2,  tileSize / 2 }, { 1, 0 } },
};

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}

namespace Canvas {

inline static std::vector<Vertex> vertices = {
    { { -1.0f, -1.0f }, { 0, -1 } },
    { {  1.0f, -1.0f }, { 1, -1 } },
    { { -1.0f,  1.0f }, { 0, 0 } },
    { {  1.0f,  1.0f }, { 1, 0 } },
};

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}

namespace Pixel {

inline static std::vector<Vertex> vertices = {
    { { -0.5f, -0.5f }, { 0, 1 } },
    { {  0.5f, -0.5f }, { 1, 1 } },
    { { -0.5f,  0.5f }, { 0, 0 } },
    { {  0.5f,  0.5f }, { 1, 0 } },
};

inline static std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}

}