#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include <map>

#include "maths/maths.h"

// #ifdef MW_USE_GLM
// #	include <glm/vec2.hpp>
// #endif

#ifdef NDEBUG
#	define MW_RELEASE
#else
#	define MW_DEBUG
#endif

#define MW_LOG_OUT(x) std::cout << x << '\n'

#define MW_VOID_ASSEMBLY ((void)0)

#ifdef MW_DEBUG
#	define MW_DEBUG_LOG_OUT(x) MW_LOG_OUT(x)
#else
#	define MW_DEBUG_LOG_OUT(x) MW_VOID_ASSEMBLY
#endif

namespace mw {

extern std::string ROOT_DIR;
void DefRoot(int amountOfArguments, char *arguments[]);

// #ifdef MW_USE_GLM
// 	using Vec2 = glm::vec2;
// #else
// 	struct Vec2
// 	{
// 		float x;
// 		float y;

// 		Vec2(float x, float y)
// 		{
// 			this->x = x;
// 			this->y = y;
// 		}
// 	};
// #endif

namespace Mem {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Ptr = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Ptr<T> CreatePtr(Args &&...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

}

using Str = std::string;
// using Size = Vec2;
// using Pos = Vec2;

}