#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ww {

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Vec2i = glm::ivec2;

using Mat4 = glm::mat4x4;
using Mat2x4 = glm::mat2x4;

using Color = Vec4;

inline Vec2 ceil(Vec2 value) { return Vec2(ceilf(value.x), ceilf(value.y)); }
inline Vec3 ceil(Vec3 value) { return Vec3(ceilf(value.x), ceilf(value.y), ceilf(value.z)); }
inline Vec2 trunc(Vec2 value) { return Vec2(truncf(value.x), truncf(value.y)); }
inline Vec3 trunc(Vec3 value) { return Vec3(truncf(value.x), truncf(value.y), truncf(value.z)); }
inline Vec2 round(Vec2 value) { return Vec2(roundf(value.x), roundf(value.y)); }
inline Vec3 round(Vec3 value) { return Vec3(roundf(value.x), roundf(value.y), roundf(value.z)); }
inline Vec2 ToInt(Vec2 value) { return Vec2(static_cast<int>(value.x), static_cast<int>(value.y)); }
inline Vec3 ToInt(Vec3 value) { return Vec3(static_cast<int>(value.x), static_cast<int>(value.y), static_cast<int>(value.z)); }

inline static void ForEachComp(float *vec, int components, std::function<float(float)> action)
{
	float *address = vec;
	for (int i = 0; i < components; i++)
	{
		float newValue = action(*address);
		memcpy(address++, &newValue, sizeof(float));
	}
}

inline static Color RED_COLOR = Color(1, 0, 0, 1);
inline static Color GREEN_COLOR = Color(0, 1, 0, 1);
inline static Color BLUE_COLOR = Color(0, 0, 1, 1);
inline static Color YELLOW_COLOR = Color(1, 1, 0, 1);
inline static Color PURPLE_COLOR = Color(1, 0, 1, 1);
inline static Color WHITE_COLOR = Color(1, 1, 1, 1);
inline static Color BLACK_COLOR = Color(0, 0, 0, 1);

#define ALIAS_TEMPLATE_FUNCTION(highLevelF, lowLevelF)                                       \
	template <typename... Args>                                                              \
	inline auto highLevelF(Args &&...args)->decltype(lowLevelF(std::forward<Args>(args)...)) \
	{                                                                                        \
		return lowLevelF(std::forward<Args>(args)...);                                       \
	}

namespace Math {

ALIAS_TEMPLATE_FUNCTION(Perspective, glm::perspective)
ALIAS_TEMPLATE_FUNCTION(Ortho, glm::ortho)
ALIAS_TEMPLATE_FUNCTION(Radians, glm::radians)
ALIAS_TEMPLATE_FUNCTION(Inverse, glm::inverse)

ALIAS_TEMPLATE_FUNCTION(ToPtr, glm::value_ptr)

ALIAS_TEMPLATE_FUNCTION(Translate, glm::translate)
ALIAS_TEMPLATE_FUNCTION(Rotate, glm::rotate)
ALIAS_TEMPLATE_FUNCTION(Scale, glm::scale)

ALIAS_TEMPLATE_FUNCTION(Distance, glm::distance)

}

namespace Physics {

inline static constexpr float g = 9.81f;

}

}