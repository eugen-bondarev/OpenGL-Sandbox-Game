#pragma once

#include <iostream>

#include <vector>
#include <memory>
#include <string>
#include <map>

#include "Util/Structures.h"
#include "Util/Memory.h"
#include "Util/Types.h"

#define VOID_ASSEMBLY ((void)0)

#define FORGIO_ENABLE_PROFILING

#ifdef FORGIO_ENABLE_PROFILING
# include "Instrumentor.h"
# define FORGIO_PROFILER_BEGIN()            Instrumentor::Get().BeginSession("Forgio")
# define FORGIO_PROFILER_END()              Instrumentor::Get().EndSession()
# define FORGIO_PROFILER_SCOPE()            InstrumentationTimer timer(__FUNCSIG__)
# define FORGIO_PROFILER_NAMED_SCOPE(NAME)  InstrumentationTimer timer(NAME)
#else
# define FORGIO_PROFILER_BEGIN()            VOID_ASSEMBLY
# define FORGIO_PROFILER_END()              VOID_ASSEMBLY
# define FORGIO_PROFILER_SCOPE()            VOID_ASSEMBLY
# define FORGIO_PROFILER_NAMED_SCOPE(NAME)  VOID_ASSEMBLY
#endif

#define FORGIO_DEVELOPMENT  0
#define FORGIO_PRODUCTION   1

#define FORGIO_BUILD_TYPE   FORGIO_DEVELOPMENT
// #define FORGIO_BUILD_TYPE   FORGIO_PRODUCTION

#if FORGIO_BUILD_TYPE == FORGIO_DEVELOPMENT
# define FORGIO_ROOT "D:/Dev/Forgio/"
#else
# define FORGIO_ROOT ""
#endif

#ifdef NDEBUG
# define FORGIO_RELEASE
#else
# define FORGIO_DEBUG
#endif

#define LOG_OUT(x) std::cout << x << '\n'

#ifdef FORGIO_DEBUG
# define DEBUG_LOG_OUT(x) LOG_OUT(x)
#else
# define DEBUG_LOG_OUT(x) VOID_ASSEMBLY
#endif