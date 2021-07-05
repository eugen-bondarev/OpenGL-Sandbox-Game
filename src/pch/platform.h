#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#   define NF_PLATFORM_WINDOWS
#else
#   define NF_PLATFORM_LINUX
#endif

#define NF_ERROR_EXIT() exit(1)