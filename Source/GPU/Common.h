#pragma once

#include <iostream>

#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include <map>

#ifdef NDEBUG
# define WERWEL_RELEASE
#else
# define WERWEL_DEBUG
#endif

#define WERWEL_LOG_OUT(x) std::cout << x << '\n'

#define WERWEL_VOID_ASSEMBLY ((void)0)

#ifdef WERWEL_DEBUG
# define WERWEL_DEBUG_LOG_OUT(x) LOG_OUT(x)
#else
# define WERWEL_DEBUG_LOG_OUT(x) VOID_ASSEMBLY
#endif

namespace Werwel {

namespace Mem {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
using Ptr = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Ptr<T> CreatePtr(Args&& ... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

}

using Str = std::string;

struct Size {
  float x;
  float y;

  Size(float x, float y) {
    this->x = x;
    this->y = y;
  }
};

}