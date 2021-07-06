#pragma once

#include <string>

#define MW_DEVELOPMENT  0
#define MW_PRODUCTION   1

#define MW_BUILD_TYPE   MW_PRODUCTION

#ifdef NDEBUG
#   define MW_RELEASE
#else
#   define MW_DEBUG
#endif