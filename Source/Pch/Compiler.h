#pragma once

#if defined(__clang__)
# define NATURAFORGE_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
# define NATURAFORGE_COMPILER_GCC
#elif defined(_MSC_VER)
# define NATURAFORGE_COMPILER_VS
#endif