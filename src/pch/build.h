#pragma once

#include <string>

#define NF_DEVELOPMENT  0
#define NF_PRODUCTION   1

#define NF_BUILD_TYPE   NF_PRODUCTION

inline std::string NF_ROOT;

#ifdef NDEBUG
#   define NF_RELEASE
#else
#   define NF_DEBUG
#endif