#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#   define MW_PLATFORM_WINDOWS
#else
#   define MW_PLATFORM_LINUX
#endif

#define MW_ERROR_EXIT() exit(1)