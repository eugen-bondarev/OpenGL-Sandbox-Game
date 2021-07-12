#pragma once

#include <Windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include <map>

#include "pch/compiler.h"
#include "pch/function.h"
#include "pch/class.h"
#include "pch/void.h"
#include "pch/instrumentor.h"
#include "pch/gpu.h"
#include "pch/build.h"
#include "pch/log.h"
#include "pch/platform.h"
#include "pch/console.h"
#include "pch/flags.h"

#include "maths/maths.h"

#ifdef NDEBUG
#	define MW_RELEASE
#else
#	define MW_DEBUG
#endif

#define MW_LOG_OUT(x) std::cout << x << '\n'
#define MW_LOG_VAR(x) MW_LOG_OUT(#x << ": " << x)

struct CheckScope
{
	std::string message;
	float time;

	CheckScope(const std::string message = "") : message { message }
	{
		time = glfwGetTime();
	}

	~CheckScope()
	{
		float elapsedTime = (glfwGetTime() - time) * 1000.0f;
		MW_LOG_OUT(message << ": " << elapsedTime << " ms");
	}
};

#define MW_VOID_ASSEMBLY ((void)0)

#ifdef MW_DEBUG
#	define MW_DEBUG_LOG_OUT(x) MW_LOG_OUT(x)
#else
#	define MW_DEBUG_LOG_OUT(x) MW_VOID_ASSEMBLY
#endif

namespace mw {

extern std::string ROOT_DIR;
void DefRoot(int amountOfArguments, char *arguments[]);

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

}