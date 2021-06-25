#pragma once

#include <iostream>

#define LOG_OUT(x) std::cout << x << '\n'

#ifdef NATURAFORGE_DEBUG
# define LOG_OUT_D(x) LOG_OUT(x)
#else
# define LOG_OUT_D(x) NATURAFORGE_VOID_ASSEMBLY
#endif