#pragma once

#include <iostream>

#include <vector>
#include <memory>
#include <string>
#include <map>

#include "Util/Structures.h"
#include "Util/Memory.h"
#include "Util/Types.h"

#include "Instrumentor.h"

/**
 * For some reason this doesn't work on Diana's PC. 
 * Need to keep that in mind.
 * Todo: Check STATIC_BUFFER on her PC.
 */
#define FORGIO_SUPPORT_DYNAMIC_BUFFER
#define FORGIO_ENABLE_PROFILING

#define FORGIO_ROOT "D:/Dev/Forgio/"
// #define FORGIO_ROOT ""

#ifdef NDEBUG
# define FORGIO_RELEASE
#else
# define FORGIO_DEBUG
#endif

#define LOG_OUT(x) std::cout << x << '\n'

// This macro doesn't create any assembly code.
#define EMPTY_MACRO ((void)0)

#ifdef FORGIO_DEBUG
# define DEBUG_LOG_OUT(x) LOG_OUT(x)
#else
# define DEBUG_LOG_OUT(x) EMPTY_MACRO
#endif

#ifdef FORGIO_ENABLE_PROFILING
# define FORGIO_PROFILER_BEGIN()            Instrumentor::Get().BeginSession("Forgio")
# define FORGIO_PROFILER_END()              Instrumentor::Get().EndSession()
# define FORGIO_PROFILER_SCOPE()            InstrumentationTimer timer(__FUNCSIG__)
# define FORGIO_PROFILER_NAMED_SCOPE(NAME)  InstrumentationTimer timer(NAME)
#else
# define FORGIO_PROFILER_BEGIN()            EMPTY_MACRO
# define FORGIO_PROFILER_END()              EMPTY_MACRO
# define FORGIO_PROFILER_SCOPE()            EMPTY_MACRO
# define FORGIO_PROFILER_NAMED_SCOPE(NAME)  EMPTY_MACRO
#endif