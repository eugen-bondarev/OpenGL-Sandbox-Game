#pragma once

#include <iostream>

#include <boxer/boxer.h>
#include <string>

#define LOG_OUT(x) std::cout << x << '\n'
#define ERR_OUT(x) std::cout << x << '\n'

#ifdef NF_DEBUG
# define LOG_OUT_D(x) LOG_OUT(x)
#else
# define LOG_OUT_D(x) NF_VOID_ASSEMBLY
#endif

#define NF_ERROR_POPUP(message, title) boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Error)
#define NF_INFO_POPUP(message, title) boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Info)
#define NF_WARN_POPUP(message, title) boxer::show(std::string(message).c_str(), std::string(title).c_str(), boxer::Style::Warning)