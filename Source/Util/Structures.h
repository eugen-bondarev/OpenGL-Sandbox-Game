#pragma once

#include "Math/Math.h"

using Size  = Vec2;
using Pos   = Vec2;

template <typename T = int>
struct Period {
    T start;
    T end;
};