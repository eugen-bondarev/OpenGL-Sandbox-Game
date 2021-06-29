#pragma once

#include <iostream>

#include <boxer/boxer.h>
#include <string>

#define LOG_OUT(x) std::cout << x << '\n'
#define ERR_OUT(x) std::cout << x << '\n'

#ifdef NATURAFORGE_DEBUG
# define LOG_OUT_D(x) LOG_OUT(x)
#else
# define LOG_OUT_D(x) NATURAFORGE_VOID_ASSEMBLY
#endif

#define NATURAFORGE_ERROR_POPUP(message, title) boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Error)
#define NATURAFORGE_INFO_POPUP(message, title) boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Info)
#define NATURAFORGE_WARN_POPUP(message, title) boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Warning)