#pragma once

// #define FORGIO_ENABLE_PROFILING
// #define FORGIO_ENABLE_GPU_SYNC

#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <array>
#include <map>

#include "Util/Structures.h"
#include "Util/Memory.h"
#include "Util/Types.h"

#define VOID_ASSEMBLY ((void)0)

#ifdef FORGIO_ENABLE_PROFILING
# include "Instrumentor.h"
# define FORGIO_PROFILER_BEGIN(NAME)        Instrumentor::Get().BeginSession(NAME)
# define FORGIO_PROFILER_END()              Instrumentor::Get().EndSession()
# define FORGIO_PROFILER_SCOPE()            InstrumentationTimer timer(__FUNCSIG__)
# define FORGIO_PROFILER_NAMED_SCOPE(NAME)  InstrumentationTimer timer(NAME)
#else
# define FORGIO_PROFILER_BEGIN(NAME)        VOID_ASSEMBLY
# define FORGIO_PROFILER_END()              VOID_ASSEMBLY
# define FORGIO_PROFILER_SCOPE()            VOID_ASSEMBLY
# define FORGIO_PROFILER_NAMED_SCOPE(NAME)  VOID_ASSEMBLY
#endif

#ifdef FORGIO_ENABLE_GPU_SYNC
# define FORGIO_SYNC_GPU()  glFinish()
#else
# define FORGIO_SYNC_GPU() VOID_ASSEMBLY
#endif

#define FORGIO_DEVELOPMENT  0
#define FORGIO_PRODUCTION   1

#define FORGIO_BUILD_TYPE   FORGIO_PRODUCTION

inline std::string FORGIO_ROOT;

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