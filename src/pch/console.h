#pragma once

#ifdef NF_PLATFORM_WINDOWS
#   define NOMINMAX
#   include <Windows.h>
#   define NF_CONSOLE_HIDE()         ::ShowWindow(::GetConsoleWindow(), SW_HIDE)
#   define NF_CONSOLE_SHOW()         ::ShowWindow(::GetConsoleWindow(), SW_SHOW)
#   define NF_IS_CONSOLE_VISIBLE()   ::IsWindowVisible(::GetConsoleWindow()) != FALSE
#endif