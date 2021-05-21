#pragma once

#include <iostream>

#include <vector>
#include <memory>
#include <string>
#include <map>

#ifdef NDEBUG
#   define PROJECTOR_RELEASE
#else
#   define PROJECTOR_DEBUG
#endif

#define LOG_OUT(x) std::cout << x << '\n'

// This macro doesn't create any assembly code.
#define EMPTY_MACRO ((void)0)

#ifdef PROJECTOR_DEBUG
#   define DEBUG_LOG_OUT(x) LOG_OUT(x)
#else
#   define DEBUG_LOG_OUT(x) EMPTY_MACRO
#endif