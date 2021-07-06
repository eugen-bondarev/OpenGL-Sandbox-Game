#pragma once

#if defined(__clang__)
#   define MW_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#   define MW_COMPILER_GCC
#elif defined(_MSC_VER)
#   define MW_COMPILER_VS
#endif