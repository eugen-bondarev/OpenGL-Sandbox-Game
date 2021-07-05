#pragma once

#if defined(__clang__)
#   define NF_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#   define NF_COMPILER_GCC
#elif defined(_MSC_VER)
#   define NF_COMPILER_VS
#endif