#pragma once

#ifdef MW_PLATFORM_WINDOWS
#   define NOMINMAX
#   include <Windows.h>
#   define MW_CONSOLE_HIDE()         ::ShowWindow(::GetConsoleWindow(), SW_HIDE)
#   define MW_CONSOLE_SHOW()         ::ShowWindow(::GetConsoleWindow(), SW_SHOW)
#   define MW_IS_CONSOLE_VISIBLE()   ::IsWindowVisible(::GetConsoleWindow()) != FALSE
#endif