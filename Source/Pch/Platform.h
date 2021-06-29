#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
# define NATURAFORGE_PLATFORM_WINDOWS
#else
# define NATURAFORGE_PLATFORM_LINUX
#endif

#define NATURAFORGE_ERROR_EXIT() exit(1)