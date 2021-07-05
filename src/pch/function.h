#pragma once

#if defined(NF_COMPILER_GCC) || defined(NF_COMPILER_CLANG)
#   define NF_FUNC_SIG __func__
#elif defined(NF_COMPILER_VS)
#   define NF_FUNC_SIG __FUNCSIG__
#else
#   define NF_FUNC_SIG "Undefined"
#endif