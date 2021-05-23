#pragma once

#include "Gpu/Vertex.h"

#include <vector>
    
namespace Primitives {
namespace Quad {

std::vector<Vertex> vertices = {
    { { -50, -50, 0 }, { 0, 1 }, { 0, 0, 0 } },
    { {  50, -50, 0 }, { 1, 1 }, { 0, 0, 0 } },
    { { -50,  50, 0 }, { 0, 0 }, { 0, 0, 0 } },
    { {  50,  50, 0 }, { 1, 0 }, { 0, 0, 0 } },
};

std::vector<int> indices = { 0, 1, 2, 2, 1, 3 };

}
}