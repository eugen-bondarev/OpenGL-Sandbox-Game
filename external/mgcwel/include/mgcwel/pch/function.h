#pragma once

#if defined(MW_COMPILER_GCC) || defined(MW_COMPILER_CLANG)
#   define MW_FUNC_SIG __func__
#elif defined(MW_COMPILER_VS)
#   define MW_FUNC_SIG __FUNCSIG__
#else
#   define MW_FUNC_SIG "Undefined"
#endif