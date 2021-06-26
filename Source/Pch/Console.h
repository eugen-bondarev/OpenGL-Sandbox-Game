#pragma once

#ifdef NATURAFORGE_PLATFORM_WINDOWS
# define NOMINMAX
# include <Windows.h>
# define NATURAFORGE_CONSOLE_HIDE()         ::ShowWindow(::GetConsoleWindow(), SW_HIDE)
# define NATURAFORGE_CONSOLE_SHOW()         ::ShowWindow(::GetConsoleWindow(), SW_SHOW)
# define NATURAFORGE_IS_CONSOLE_VISIBLE()   ::IsWindowVisible(::GetConsoleWindow()) != FALSE
#endif