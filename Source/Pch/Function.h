#pragma once

#if defined(NATURAFORGE_COMPILER_GCC) || defined(NATURAFORGE_COMPILER_CLANG)
# define NATURAFORGE_FUNC_SIG __func__
#elif defined(NATURAFORGE_COMPILER_VS)
# define NATURAFORGE_FUNC_SIG __FUNCSIG__
#else
# define NATURAFORGE_FUNC_SIG "Undefined"
#endif