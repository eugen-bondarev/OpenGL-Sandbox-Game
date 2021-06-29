#pragma once

/**
 * Math implementation
 */
#include "Maths/Maths.h"

#define IM_VEC2_CLASS_EXTRA                                               \
        ImVec2(const Vec2& f) { x = f.x; y = f.y; }                       \
        operator Vec2() const { return Vec2(x,y); }